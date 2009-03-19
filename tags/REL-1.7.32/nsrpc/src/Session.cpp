#include "stdafx.h"
#include "BandwidthLimit.h"
#include <nsrpc/Session.h>
#include <nsrpc/SessionConfig.h>
#include <nsrpc/detail/Asynch_RW.h>
#include <nsrpc/detail/SessionDestroyer.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/CsProtocol.h>
#include <nsrpc/utility/Logger.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <ace/SOCK_Connector.h>
#include <ace/SOCK_Stream.h>

namespace nsrpc
{

// = Session

Session::Session(const SessionConfig& config) :
    sessionDestroyer_(*config.sessionDestroyer_),
    messageBlockManager_(*config.messageBlockManager_),
    packetCoder_(config.packetCoder_),
    proactor_(config.proactor_),
    inboundBandwidthLimiter_(new BandwidthLimit(config.capacity_))
{
    recvBlock_ =
        messageBlockManager_.create(packetCoder_->getDefaultPacketSize());

    reset();
}


Session::~Session()
{
    recvBlock_->release();
}


bool Session::connect(const srpc::String& host, u_short port, size_t timeout)
{
    const ACE_INET_Addr address(port, host.c_str());
    const ACE_Time_Value connectionTimeout(makeTimeValue(timeout));

    ACE_SOCK_Stream stream;
    ACE_SOCK_Connector connector;
    if (connector.connect(stream, address, &connectionTimeout) == -1) {
        NSRPC_LOG_ERROR(ACE_TEXT("Connection Failed(%m)."));
        return false;
    }

    ACE_Message_Block* dummyMessageBlock = 0;
    open(stream.get_handle(), *dummyMessageBlock);
    stream.set_handle(ACE_INVALID_HANDLE);

    return isConnected();
}


// CAUTION: RPC 핸들러(RpcEvent) 내에서 RPC를 호출하고 전송이 실패할 경우
// disconnect()를 호출하면 RPC 핸들러의 코드가 깨지는 경우가 발생한다.
// 따라서 disconnect()를 호출해선 안된다.
void Session::sendMessage(ACE_Message_Block& mblock,
    CsMessageType messageType)
{
    AceMessageBlockGuard block(&mblock);

    ACE_GUARD(ACE_Thread_Mutex, monitor, sendLock_);

    if (! isConnected()) {
        return;
    }

    if (! packetCoder_->isValidPacket(*block)) {
        NSRPC_LOG_DEBUG2(ACE_TEXT("Session::sendMessage() - ")
            ACE_TEXT("Too short message(%d)."), block->length());
        return;
    }

    CsPacketHeader header(messageType);
    if (! packetCoder_->encode(*block, header)) {
        NSRPC_LOG_DEBUG(ACE_TEXT("Session::sendMessage() encoding FAILED."));
        return;
    }

    if (! write(*block)) {
        return;
    }

    block.release();
}


void Session::disconnect()
{
    bool connected = false;
    {
        ACE_GUARD(ACE_Thread_Mutex, recvMonitor, recvLock_);
        ACE_GUARD(ACE_Thread_Mutex, sendMonitor, sendLock_);

        stopDisconnectTimer();
        stopThrottleTimer();

        connected = isConnected();
        stream_->cancel();
        stream_->close();
    }

    if (connected) {
        onDisconnected();

        NSRPC_LOG_DEBUG3(ACE_TEXT("Session Stats - %d/%d."),
            stats_.sentBytes_, stats_.recvBytes_);
        sessionDestroyer_.release(this);
    }
}


void Session::disconnectGracefully()
{
    ACE_GUARD(ACE_Thread_Mutex, recvMonitor, recvLock_);
    ACE_GUARD(ACE_Thread_Mutex, sendMonitor, sendLock_);

    if (! disconnectReserved_) {
        disconnectReserved_ = true;
        stream_->shutdown();
        startDisconnectTimer();
    }
}


bool Session::isConnected() const
{
    return isValidHandle(stream_->getHandle());
}


bool Session::readMessage(const NSRPC_Asynch_Read_Stream::Result& result)
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, monitor, recvLock_, false);

    const size_t bytesTransferred = result.bytes_transferred();
    stats_.recvBytes_ += bytesTransferred;
    assert(result.bytes_to_read() >= bytesTransferred);

    inboundBandwidthLimiter_->add(bytesTransferred);
    const bool isKarmaRemained = inboundBandwidthLimiter_->check();
    //NSRPC_LOG_DEBUG3("BandwidthLimit left bytes = %d(-%d)",
    //    inboundBandwidthLimiter_->getLeftBytes(), bytes_transferred);

    ACE_Message_Block& mblock = result.message_block();
    assert(&mblock == recvBlock_);

    const size_t neededBytes = result.bytes_to_read() - bytesTransferred;
    if (neededBytes > 0) {
        return readMessageBody(neededBytes);
    }

    if (mblock.length() == packetCoder_->getHeaderSize()) {
        if (packetCoder_->readHeader(headerForReceive_, mblock)) {
            return readMessageBody(headerForReceive_.bodySize_);
        }
        else {
            NSRPC_LOG_DEBUG(ACE_TEXT("Session::readMessage() - ")
                ACE_TEXT("Invalid Message Header(%m)."));
            return false;
        }
    }

    if (! packetCoder_->decode(mblock)) {
        return false;
    }

    packetCoder_->advanceToBody(mblock);

    if (! isValidCsMessageType(headerForReceive_.messageType_)) {
        NSRPC_LOG_DEBUG(ACE_TEXT("Session::readMessage() - ")
            ACE_TEXT("Invalid Message Type."));
        return false;
    }

    if (! onMessageArrived(headerForReceive_.messageType_)) {
        return false;
    }

    if (isKarmaRemained) {
        return readMessageHeader();
    }
    else {
        startThrottleTimer();
        onThrottling(inboundBandwidthLimiter_->getReadBytes(),
            inboundBandwidthLimiter_->getMaxBytesPerSecond());
    }

    return true;
}


bool Session::readMessageHeader()
{
    assert(recvBlock_->size() > packetCoder_->getHeaderSize());

    recvBlock_->reset();
    return read(packetCoder_->getHeaderSize());
}


bool Session::readMessageBody(size_t neededBytes)
{
    if (recvBlock_->space() < neededBytes) {
        recvBlock_->size(recvBlock_->size() + neededBytes);
    }
    return read(neededBytes);
}


bool Session::read(size_t neededBytes)
{
    if (stream_->read(*recvBlock_, neededBytes) != 0) {
        NSRPC_LOG_DEBUG2(ACE_TEXT("Session::read() - ")
            ACE_TEXT("Asynch_RW_Stream::read(%d) FAILED(%m)."),
            neededBytes);
        return false;
    }

    ++pendingReadCount_;
    return true;
}


bool Session::write(ACE_Message_Block& mblock)
{
    if (stream_->write(mblock, mblock.length()) != 0) {
        NSRPC_LOG_DEBUG4(ACE_TEXT("Session(0x%X)::write() - ")
            ACE_TEXT("Asynch_RW_Stream::write(%d) FAILED(%m, %d)."),
            this, mblock.length(), errno);
        return false;
    }

    ++pendingWriteCount_;
    return true;
}


void Session::reset()
{
    stream_.reset(new Asynch_RW_Stream);
    stats_.reset();
    pendingReadCount_ = pendingWriteCount_ = 0;
    prevLoggedPendingWriteCount_ = 0;
    disconnectReserved_ = false;
    disconnectTimer_ = -1;
    throttleTimer_ = -1;
    packetCoder_->reset();
    inboundBandwidthLimiter_->reset();
}


void Session::startDisconnectTimer()
{
    assert(disconnectTimer_ == -1);

    const size_t disconnectTimeout = 500;

    disconnectTimer_ = setupTimer(*proactor(), *this,
        disconnectTimeout, &disconnectTimer_);
}


void Session::stopDisconnectTimer()
{
    if (disconnectTimer_ != -1) {
        cancelTimer(*proactor(), disconnectTimer_);
    }
}


void Session::startThrottleTimer()
{
    if (throttleTimer_ != -1) {
        return;
    }

    const size_t throttleTimeout =
        inboundBandwidthLimiter_->getSecondsForThrottling() * 1000;

    throttleTimer_ = setupTimer(*proactor(), *this,
        throttleTimeout, &throttleTimer_);
}


void Session::stopThrottleTimer()
{
    if (throttleTimer_ != -1) {
        cancelTimer(*proactor(), throttleTimer_);
    }
}


void Session::checkPendingCount()
{
    const long logThreshold = 10;
    const long maxThreshold = 1000;

    const long writeCount = pendingWriteCount_.value();

    {
        ACE_GUARD(ACE_Thread_Mutex, sendMonitor, sendLock_);

        const long diffWrite =
            std::abs(writeCount - prevLoggedPendingWriteCount_);
        if (diffWrite > logThreshold) {
            NSRPC_LOG_INFO3("Session(0x%X): %d output pending.",
                this, writeCount);
            prevLoggedPendingWriteCount_ = writeCount;
        }
    }

    if (writeCount >= maxThreshold) {
        if (! isConnected()) {
            return;
        }

        NSRPC_LOG_ERROR3(
            "Session(0x%X): will disconnect - Too many output pending(%d).",
            this, writeCount);
        disconnect();
    }
}


void Session::onThrottling(size_t readBytes, size_t maxBytesPerSecond)
{
    ACE_TCHAR address[MAXHOSTNAMELEN];
    NSRPC_LOG_INFO6("Client(%s:%d) is THROTTLED(%d > %d), "
        "delaying read(%d sec).",
        remoteAddress_.get_host_addr(address, MAXHOSTNAMELEN),
        remoteAddress_.get_port_number(),
        readBytes, maxBytesPerSecond,
        inboundBandwidthLimiter_->getSecondsForThrottling());
}

// = NSRPC_Service_Handler overriding

void Session::open(ACE_HANDLE new_handle, ACE_Message_Block& /*message_block*/)
{
    {
        ACE_GUARD(ACE_Thread_Mutex, recvMonitor, recvLock_);
        ACE_GUARD(ACE_Thread_Mutex, sendMonitor, sendLock_);

        assert(isSafeToDelete());
        reset();

        if (stream_->open(*this, new_handle, 0, proactor_, true)) {
            if (readMessageHeader()) {
                onConnected();
                return; // success
            }
        }

        NSRPC_LOG_DEBUG(ACE_TEXT("Session::open() - FAILED(%m)."));
    }

    disconnect();
}


void Session::handle_read_stream(
    const NSRPC_Asynch_Read_Stream::Result& result)
{
    --pendingReadCount_;

    if (result.success()) {
        if (result.bytes_transferred() > 0) {
            if (readMessage(result)) {
                return; // success
            }
        }
        //else {
        //    NSRPC_LOG_DEBUG(ACE_TEXT("The client close the socket."));
        //}
    }
    //else if (! stream_->cancelled()) {
    //    NSRPC_LOG_DEBUG(ACE_TEXT("Session::handle_read_stream() - ")
    //        ACE_TEXT("FAILED(%m)."));
    //}

    disconnect();
}


void Session::handle_write_stream(
    const NSRPC_Asynch_Write_Stream::Result& result)
{
    --pendingWriteCount_;

    result.message_block().release();

    const size_t bytesTransferred = result.bytes_transferred();
    if ((result.error() == 0) && (bytesTransferred > 0)) {
        assert(result.bytes_to_write() == bytesTransferred);
        stats_.sentBytes_ += bytesTransferred;
        checkPendingCount();
        return; // success
    }

    if (! stream_->cancelled()) {
        NSRPC_LOG_DEBUG(ACE_TEXT("Session::handle_write_stream() - ")
            ACE_TEXT("FAILED(%m)."));
    }

    disconnect();
}


void Session::handle_time_out(const ACE_Time_Value& /*tv*/, const void* act)
{
    if (act == &disconnectTimer_) {
        NSRPC_LOG_DEBUG(ACE_TEXT("Disconnect timer fired"));
        disconnectTimer_ = -1;
        if (pendingWriteCount_ > 0) {
            startDisconnectTimer();
        }
        else {
            disconnect();
        }
    }
    else if (act == &throttleTimer_) {
        throttleTimer_ = -1;
        inboundBandwidthLimiter_->reset();
        if (! readMessageHeader()) {
            disconnect();
        }
    }
}


void Session::addresses(const ACE_INET_Addr& remote_address,
    const ACE_INET_Addr& local_address)
{
    remoteAddress_ = remote_address;
    localAddress_ = local_address;
}

// = MessageBlockProvider overriding

ACE_Message_Block& Session::acquireSendBlock()
{
    ACE_Message_Block* mblock =
        messageBlockManager_.create(packetCoder_->getDefaultPacketSize());
    packetCoder_->reserveHeader(*mblock);
    return *mblock;
}


ACE_Message_Block& Session::acquireRecvBlock()
{
    return *recvBlock_;
}

} // namespace nsrpc
