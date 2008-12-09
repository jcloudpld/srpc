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
    recvBlock_ = new ACE_Message_Block(packetCoder_->getMaxPacketSize());
        //messageBlockManager_.create(packetCoder_->getMaxPacketSize());
    msgBlock_ = new ACE_Message_Block(packetCoder_->getMaxPacketSize());
        //messageBlockManager_.create(packetCoder_->getMaxPacketSize());

    reset();
}


Session::~Session()
{
    recvBlock_->release();
    msgBlock_->release();
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
    if (! isConnected()) {
        return;
    }

    AceMessageBlockGuard block(&mblock);

    ACE_GUARD(ACE_Thread_Mutex, monitor, sendLock_);

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
    assert(&(result.message_block()) == recvBlock_);

    inboundBandwidthLimiter_->add(bytesTransferred);

    if (! handleMessages()) {
        return false;
    }

    const bool isKarmaRemained = inboundBandwidthLimiter_->check();
    if (isKarmaRemained) {
        //NSRPC_LOG_DEBUG3("BandwidthLimit left bytes = %d(-%d)",
        //    inboundBandwidthLimiter_->getLeftBytes(), bytesTransferred);
        return beginToRead();
    }
    else {
        startThrottleTimer();
        onThrottling(inboundBandwidthLimiter_->getReadBytes(),
            inboundBandwidthLimiter_->getMaxBytesPerSecond());
    }

    return true;
}


bool Session::handleMessages()
{
    while (isPacketHeaderArrived()) {
        if (! parseHeader()) {
            return false;
        }

        if (! isMessageArrived()) {
            break;
        }

        if (! parseMessage()) {
            return false;
        }

        if (! isValidCsMessageType(headerForReceive_.messageType_)) {
            NSRPC_LOG_ERROR(ACE_TEXT("Session::readMessage() - ")
                ACE_TEXT("Invalid Message Type."));
            return false;
        }

        if (! onMessageArrived(headerForReceive_.messageType_)) {
            return false;
        }
    }

    return true;
}


bool Session::parseHeader()
{
    assert(recvBlock_->length() >= packetCoder_->getHeaderSize());
    if (! packetCoder_->readHeader(headerForReceive_, *recvBlock_)) {
        return false;
    }

    return true;
}


bool Session::parseMessage()
{
    const size_t messageSize =
        packetCoder_->getHeaderSize() + headerForReceive_.bodySize_;

    msgBlock_->reset();
    if (msgBlock_->space() < messageSize) {
        msgBlock_->size(messageSize);
    }
    msgBlock_->copy(recvBlock_->base(), messageSize);

    recvBlock_->rd_ptr(messageSize);
    recvBlock_->crunch();

    if (! packetCoder_->decode(*msgBlock_)) {
        return false;
    }
    packetCoder_->advanceToBody(*msgBlock_);

    return true;
}


bool Session::beginToRead()
{
    if (recvBlock_->space() <= 0) {
        const size_t spare = recvBlock_->capacity() / 10;
        assert(spare > 0);
        recvBlock_->size(recvBlock_->size() + spare);
    }

    return read(recvBlock_->space());
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
        NSRPC_LOG_DEBUG2(ACE_TEXT("Session::write() - ")
            ACE_TEXT("Asynch_RW_Stream::write(%d) FAILED(%m)."),
            mblock.length());
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
    prevPendingReadCount_ = prevPendingWriteCount_ = 0;
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


void Session::logPendingCount()
{
    const long threshold = 5;

    const long readCount = pendingReadCount_.value();
    const long writeCount = pendingWriteCount_.value();

    const long diffRead = std::abs(readCount - prevPendingReadCount_);
    const long diffWrite = std::abs(writeCount - prevPendingWriteCount_);
    if ((diffRead > threshold) || (diffWrite > threshold)) {
        NSRPC_LOG_INFO4("Session(0x%X): %d/%d(I/O) pending.",
            this, readCount, writeCount);
    }
}


bool Session::isPacketHeaderArrived() const
{
    return recvBlock_->length() >= packetCoder_->getHeaderSize();
}


bool Session::isMessageArrived() const
{
    assert(headerForReceive_.isValid());

    return recvBlock_->length() >=
        (packetCoder_->getHeaderSize() + headerForReceive_.bodySize_);
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
            if (beginToRead()) {
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
                logPendingCount();
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
        logPendingCount();
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
        if (! beginToRead()) {
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
         new ACE_Message_Block(packetCoder_->getDefaultPacketSize());
        //messageBlockManager_.create(packetCoder_->getDefaultPacketSize());
    packetCoder_->reserveHeader(*mblock);
    return *mblock;
}


ACE_Message_Block& Session::acquireRecvBlock()
{
    return *msgBlock_;
}

} // namespace nsrpc
