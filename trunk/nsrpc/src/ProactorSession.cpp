#include "stdafx.h"

#if defined (NSRPC_HAS_PROACTOR)

#include "BandwidthLimit.h"
#include <nsrpc/ProactorSession.h>
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

ProactorSession::ProactorSession(const SessionConfig& config) :
    sessionDestroyer_(*config.sessionDestroyer_),
    messageBlockManager_(*config.messageBlockManager_),
    proactor_(config.proactor_),
    packetCoder_(config.packetCoder_),
    inboundBandwidthLimiter_(new BandwidthLimit(config.capacity_)),
    recvBlock_(
        messageBlockManager_.create(packetCoder_->getDefaultPacketSize())),
    msgBlock_(
        messageBlockManager_.create(packetCoder_->getDefaultPacketSize())),
    packetHeaderSize_(config.packetCoder_->getHeaderSize())
{
    reset();
}


ProactorSession::~ProactorSession()
{
}


bool ProactorSession::connect(const srpc::String& host, u_short port, size_t timeout)
{
    const ACE_INET_Addr address(port, host.c_str());
    const ACE_Time_Value connectionTimeout(makeTimeValue(timeout));

    ACE_SOCK_Stream stream;
    ACE_SOCK_Connector connector;
    if (connector.connect(stream, address, &connectionTimeout) == -1) {
        NSRPC_LOG_ERROR2(ACE_TEXT("ProactorSession(0x%X) - Connection Failed(%m)."),
            this);
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
void ProactorSession::sendMessage(ACE_Message_Block& mblock,
    CsMessageType messageType)
{
    AceMessageBlockGuard block(&mblock);

    ACE_GUARD(ACE_Recursive_Thread_Mutex, monitor, lock_);

    if (! canSendMessage()) {
        return;
    }

    if (! packetCoder_->isValidPacket(*block)) {
        NSRPC_LOG_DEBUG3(
            ACE_TEXT("ProactorSession(0x%X)::sendMessage() - Too short message(%d)."),
            this, block->length());
        disconnectGracefully();
        return;
    }

    CsPacketHeader header(messageType);
    if (! packetCoder_->encode(*block, header)) {
        NSRPC_LOG_DEBUG2(
            ACE_TEXT("ProactorSession(0x%X)::sendMessage() encoding FAILED."),
            this);
        disconnectGracefully();
        return;
    }

    if (! write(*block)) {
        disconnectGracefully();
        return;
    }

    block.release();
}


void ProactorSession::disconnect()
{
    bool connected = false;
    {
        ACE_GUARD(ACE_Recursive_Thread_Mutex, recvMonitor, lock_);

        stopDisconnectTimer();
        stopThrottleTimer();

        connected = isConnected();
        stream_->cancel();
        stream_->close();
    }

    if (connected) {
        onDisconnected();

        NSRPC_LOG_DEBUG7(
            ACE_TEXT("ProactorSession(0x%X) Stats: sent(%u/%u), recv(%u/%u)."),
            this,
            stats_.sentBytes_, stats_.sentMessageCount_,
            stats_.recvBytes_, stats_.recvMessageCount_,
            stats_.useCount_);
        sessionDestroyer_.release(this);
    }
}


void ProactorSession::disconnectGracefully()
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, recvMonitor, lock_);

    if (! disconnectReserved_) {
        disconnectReserved_ = true;
        shouldBlockWrite_ = true;
        stream_->shutdown();
        startDisconnectTimer();
    }
}


bool ProactorSession::isConnected() const
{
    return isValidHandle(stream_->getHandle());
}


bool ProactorSession::open(ACE_HANDLE new_handle)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, recvMonitor, lock_, false);

    assert(isSafeToDelete());
    reset();

    ++stats_.useCount_;

    if (! stream_->open(*this, new_handle, 0, proactor_, true)) {
        NSRPC_LOG_DEBUG3(ACE_TEXT("ProactorSession(0x%X)::open() - FAILED(%m,%d)."),
            this, errno);
        return false;
    }

    shouldBlockWrite_  = false;

    if (! onConnected()) {
        return false;
    }

    return true;
}


// @pre 재진입이 안된다
bool ProactorSession::messageBlockReceived(
    const NSRPC_Asynch_Read_Stream::Result& result)
{
    const size_t bytesTransferred = result.bytes_transferred();
    stats_.recvBytes_ += bytesTransferred;
    assert(result.bytes_to_read() >= bytesTransferred);

    const bool isKarmaRemained = addInboundBandwidth(bytesTransferred);

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

        if (! onMessageArrived(headerForReceive_.messageType_)) {
            return false;
        }

        headerForReceive_.reset();
    }

    if (isKarmaRemained) {
        return readMessage();
    }

    throttleReceiving();
    return true;
}


bool ProactorSession::readMessage()
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

    return readMessage_i();
}


bool ProactorSession::readMessage_i()
{
    if (recvBlock_->space() <= 0) {
        const size_t spare = recvBlock_->capacity() / 50;
        assert(spare > 0);
        recvBlock_->size(recvBlock_->size() + spare);
    }

    return read(recvBlock_->space());
}


bool ProactorSession::read(size_t neededBytes)
{
    if (stream_->read(*recvBlock_, neededBytes) != 0) {
        if (0 != ACE_OS::last_error()) {
            NSRPC_LOG_DEBUG4(ACE_TEXT("ProactorSession(0x%X)::read() - ")
                ACE_TEXT("Asynch_RW_Stream::read(%d) FAILED(%m, %d)."),
                this, neededBytes, errno);
        }
        return false;
    }

    ++pendingReadCount_;
    return true;
}


bool ProactorSession::write(ACE_Message_Block& mblock)
{
    if (stream_->write(mblock, mblock.length()) != 0) {
        if (0 != ACE_OS::last_error()) {
            NSRPC_LOG_DEBUG4(ACE_TEXT("ProactorSession(0x%X)::write() - ")
                ACE_TEXT("Asynch_RW_Stream::write(%d) FAILED(%m, %d)."),
                this, mblock.length(), errno);
        }
        return false;
    }

    ++pendingWriteCount_;
    return true;
}


void ProactorSession::reset()
{
    stream_.reset(new Asynch_RW_Stream);
    stats_.reset();
    pendingReadCount_ = pendingWriteCount_ = 0;
    prevLoggedPendingWriteCount_ = 0;
    disconnectReserved_ = false;
    disconnectTimer_ = -1;
    throttleTimer_ = -1;
    packetCoder_->reset();
    headerForReceive_.reset();
    inboundBandwidthLimiter_->reset();
    shouldBlockWrite_ = true;
}


void ProactorSession::startDisconnectTimer()
{
    assert(disconnectTimer_ == -1);

    const size_t disconnectTimeout = 500;

    disconnectTimer_ = setupTimer(*proactor(), *this,
        disconnectTimeout, &disconnectTimer_);
}


void ProactorSession::stopDisconnectTimer()
{
    if (disconnectTimer_ != -1) {
        cancelTimer(*proactor(), disconnectTimer_);
    }
}


void ProactorSession::throttleReceiving()
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, monitor, lock_);

    startThrottleTimer();
    onThrottling(inboundBandwidthLimiter_->getReadBytes(),
        inboundBandwidthLimiter_->getMaxBytesPerSecond());
}


void ProactorSession::startThrottleTimer()
{
    if (throttleTimer_ != -1) {
        return;
    }

    const size_t throttleTimeout =
        inboundBandwidthLimiter_->getSecondsForThrottling() * 1000;

    throttleTimer_ = setupTimer(*proactor(), *this,
        throttleTimeout, &throttleTimer_);
}


void ProactorSession::stopThrottleTimer()
{
    if (throttleTimer_ != -1) {
        cancelTimer(*proactor(), throttleTimer_);
    }
}


void ProactorSession::checkPendingWriteCount()
{
    const long logThreshold = 10;
    const long maxThreshold = 1000;

    const long writeCount = pendingWriteCount_.value();

    {
        ACE_GUARD(ACE_Recursive_Thread_Mutex, sendMonitor, lock_);

        const long diffWrite =
            std::abs(writeCount - prevLoggedPendingWriteCount_);
        if (diffWrite > logThreshold) {
            NSRPC_LOG_INFO3("ProactorSession(0x%X): %d output pending.",
                this, writeCount);
            prevLoggedPendingWriteCount_ = writeCount;
        }
    }

    if (writeCount >= maxThreshold) {
        if (! isConnected()) {
            return;
        }

        NSRPC_LOG_WARNING3(
            "ProactorSession(0x%X): will disconnect - Too many output pending(%d).",
            this, writeCount);
        disconnect();
    }
}


bool ProactorSession::addInboundBandwidth(size_t bytesTransferred)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

    inboundBandwidthLimiter_->add(bytesTransferred);
    const bool isKarmaRemained = inboundBandwidthLimiter_->check();
    //NSRPC_LOG_DEBUG4("ProactorSession(0x%X) - BandwidthLimit left bytes = %d(-%d)",
    //    this, inboundBandwidthLimiter_->getLeftBytes(), bytes_transferred);
    return isKarmaRemained;
}


bool ProactorSession::parseHeader()
{
    if (headerForReceive_.isValid()) {
        return true;
    }

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

    assert(recvBlock_->length() >= packetHeaderSize_);
    if (packetCoder_->readHeader(headerForReceive_, *recvBlock_)) {
        return true;
    }

    NSRPC_LOG_DEBUG2(
        ACE_TEXT("ProactorSession(0x%X)::parseHeader() - ")
        ACE_TEXT("Invalid Message Header(%m)."),
        this);
    return false;
}


bool ProactorSession::parseMessage()
{
    const size_t messageSize = packetHeaderSize_ + headerForReceive_.bodySize_;

    {
        ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

        msgBlock_->reset();
        msgBlock_->size(messageSize);
        msgBlock_->copy(recvBlock_->rd_ptr(), messageSize);

        recvBlock_->rd_ptr(messageSize);
        recvBlock_->crunch();

        if (! packetCoder_->decode(*msgBlock_)) {
            return false;
        }
        packetCoder_->advanceToBody(*msgBlock_);
    }

    if (! isValidCsMessageType(headerForReceive_.messageType_)) {
        NSRPC_LOG_ERROR(ACE_TEXT("ProactorSession::parseMessage() - ")
            ACE_TEXT("Invalid Message Type."));
        return false;
    }

    ++stats_.recvMessageCount_;

    return true;
}


bool ProactorSession::isPacketHeaderArrived() const
{
    return recvBlock_->length() >= packetHeaderSize_;
}


bool ProactorSession::isMessageArrived() const
{
    return recvBlock_->length() >=
        (packetHeaderSize_ + headerForReceive_.bodySize_);
}


void ProactorSession::onThrottling(size_t readBytes, size_t maxBytesPerSecond)
{
    ACE_TCHAR address[MAXHOSTNAMELEN];
    NSRPC_LOG_INFO7("ProactorSession(0x%X) - Client(%s:%d) is THROTTLED(%d > %d), "
        "delaying read(%d sec).",
        this,
        remoteAddress_.get_host_addr(address, MAXHOSTNAMELEN),
        remoteAddress_.get_port_number(),
        readBytes, maxBytesPerSecond,
        inboundBandwidthLimiter_->getSecondsForThrottling());
}

// = NSRPC_Service_Handler overriding

void ProactorSession::open(ACE_HANDLE new_handle,
    ACE_Message_Block& /*message_block*/)
{
    if (! open(new_handle)) {
        disconnect();
        return;
    }

    if (! readMessage()) {
        disconnect();
        return;
    }

    NSRPC_LOG_DEBUG2(ACE_TEXT("ProactorSession(0x%X) opened."), this);
}


void ProactorSession::handle_read_stream(
    const NSRPC_Asynch_Read_Stream::Result& result)
{
    --pendingReadCount_;

    if (result.success()) {
        if (result.bytes_transferred() > 0) {
            if (messageBlockReceived(result)) {
                return; // success
            }
        }
        //else {
        //    NSRPC_LOG_DEBUG2(
        //        ACE_TEXT("ProactorSession(0x%X) - The client close the socket."),
        //        this);
        //}
    }
    //else if (! stream_->cancelled()) {
    //    NSRPC_LOG_DEBUG2(ACE_TEXT("ProactorSession(0x%X)::handle_read_stream() - ")
    //        ACE_TEXT("FAILED(%m)."),
    //        this);
    //}

    disconnect();
}


void ProactorSession::handle_write_stream(
    const NSRPC_Asynch_Write_Stream::Result& result)
{
    --pendingWriteCount_;

    result.message_block().release();

    const size_t bytesTransferred = result.bytes_transferred();
    if ((result.error() == 0) && (bytesTransferred > 0)) {
        assert(result.bytes_to_write() == bytesTransferred);
        ++stats_.sentMessageCount_;
        stats_.sentBytes_ += bytesTransferred;
        checkPendingWriteCount();
        return; // success
    }

    if (! stream_->cancelled()) {
        NSRPC_LOG_DEBUG2(
            ACE_TEXT("ProactorSession(0x%X)::handle_write_stream() - ")
            ACE_TEXT("FAILED(%m)."),
            this);
    }

    disconnect();
}


void ProactorSession::handle_time_out(const ACE_Time_Value& /*tv*/,
    const void* act)
{
    bool shouldDisconnect = false;
    {
        ACE_GUARD(ACE_Recursive_Thread_Mutex, recvMonitor, lock_);

        if (act == &disconnectTimer_) {
            disconnectTimer_ = -1;
            if (pendingWriteCount_ > 0) {
                startDisconnectTimer();
            }
            else {
                shouldDisconnect = true;
            }
        }
        else if (act == &throttleTimer_) {
            throttleTimer_ = -1;
            inboundBandwidthLimiter_->reset();
            if (! readMessage_i()) {
                shouldDisconnect = true;
            }
        }
    }

    if (shouldDisconnect) {
        disconnect();
    }
}


void ProactorSession::addresses(const ACE_INET_Addr& remote_address,
    const ACE_INET_Addr& local_address)
{
    remoteAddress_ = remote_address;
    localAddress_ = local_address;
}

// = MessageBlockProvider overriding

ACE_Message_Block& ProactorSession::acquireSendBlock()
{
    ACE_Message_Block* mblock =
        messageBlockManager_.create(packetCoder_->getDefaultPacketSize());
    packetCoder_->reserveHeader(*mblock);
    return *mblock;
}


ACE_Message_Block& ProactorSession::acquireRecvBlock()
{
    assert(msgBlock_.get() != 0);
    return *msgBlock_;
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
