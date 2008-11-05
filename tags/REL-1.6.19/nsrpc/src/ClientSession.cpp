#include "stdafx.h"
#include <nsrpc/detail/ClientSession.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/CsProtocol.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
#ifdef _MSC_VER
#  pragma warning( push )
#  pragma warning( disable : 4127 4251 4541 4511 4512 4800 )
#endif
#include <ace/INET_Addr.h>
#include <ace/SOCK_Connector.h>
#include <ace/Connector.h>
#include <ace/Reactor.h>
#ifdef _MSC_VER
#  pragma warning( pop )
#endif
#include <cassert>

namespace nsrpc
{

namespace
{

InitAce initAce;

} // namespace

// = ClientSession

#ifdef _MSC_VER
#  pragma warning ( push )
#  pragma warning ( disable: 4355 )
#endif

ClientSession::ClientSession(ACE_Reactor* reactor,
    PacketCoderFactory* packetCoderFactory) :
    ACE_Event_Handler(reactor),
    notifier_(reactor, this, ACE_Event_Handler::WRITE_MASK),
    disconnectReserved_(false),
    fireEventAfterFlush_(false),
    lastLogTime_(time(0)),
    prevQueueSize_(0)
{
    if (packetCoderFactory != 0) {
        packetCoder_.reset(packetCoderFactory->create());
    }
    else {
        packetCoder_.reset(PacketCoderFactory().create());
    }

    messageBlockManager_.reset(new SynchMessageBlockManager(
        packetCoder_->getDefaultPacketPoolSize(),
        packetCoder_->getDefaultPacketSize()));
    recvBlock_ =
        messageBlockManager_->create(packetCoder_->getMaxPacketSize());
    msgBlock_ =
        messageBlockManager_->create(packetCoder_->getMaxPacketSize());
}

#ifdef _MSC_VER
#  pragma warning ( pop )
#endif

ClientSession::~ClientSession()
{
    disconnect();

    reactor(0);

    releaseMessageBlocks();
}


bool ClientSession::connect(const srpc::String& ip, u_short port,
    size_t timeout)
{
    const ACE_INET_Addr address(port, ip.c_str());
    const ACE_Time_Value connectionTimeout(makeTimeValue(timeout));

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

    if (isConnected()) {
        assert(false && "Already connected. Disconnect first.");
        return false;
    }

    ACE_SOCK_Connector connector;
    if (connector.connect(peer(), address, &connectionTimeout) == -1) {
        NSRPC_LOG_ERROR4(ACE_TEXT("ClientSession::connect(%s:%u) ")
            ACE_TEXT("FAILED!!!(%d,%m)\n"),
            ip.c_str(), port, ACE_OS::last_error());
        disconnect_i(false);
        return false;
    }

    onConnected();

    if (! initSession()) {
        disconnect_i(false);
        return false;
    }

    return true;
}


void ClientSession::disconnect(bool fireEvent)
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, monitor, lock_);

    disconnect_i(fireEvent);
}


void ClientSession::disconnectGracefully(bool fireEvent)
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, monitor, lock_);

    if (! msgQueue_.is_empty()) {
        disconnectReserved_ = true;
        fireEventAfterFlush_ = fireEvent;
    }
    else {
        disconnect_i(fireEvent);
    }
}


bool ClientSession::sendMessage(ACE_Message_Block& mblock,
    CsMessageType messageType)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

    if (! isConnected()) {
        return false;
    }

    if (disconnectReserved_) {
        return false;
    }

    AceMessageBlockGuard block(&mblock);

    if (packetCoder_->isValidPacket(*block)) {
        CsPacketHeader header(messageType);
        if (packetCoder_->encode(*block, header)) {
            //ACE_Time_Value nowait(ACE_OS::gettimeofday());
            const int result = msgQueue_.enqueue_tail(block.get(), 0); //&nowait);
            if (result != -1) {
                block.release();
                return true; // success
            }
            else {
                NSRPC_LOG_ERROR(ACE_TEXT("ClientSession::sendMessage() - ")
                    ACE_TEXT("enqueue_tail() FAILED."));
            }
        }
        else {
            NSRPC_LOG_ERROR(ACE_TEXT("ClientSession::sendMessage() - ")
                ACE_TEXT("Packet encoding FAILED."));
        }
    }
    else {
        NSRPC_LOG_ERROR2(ACE_TEXT("ClientSession::sendMessage() - ")
            ACE_TEXT("Too short message(%d)."), block->length());
    }

    disconnect_i(true);
    return false;
}


PacketCoder& ClientSession::getPacketCoder()
{
    return *packetCoder_;
}


bool ClientSession::initSession()
{
    packetCoder_->reset();

    long nagle = 1;
    stream_.set_option(IPPROTO_TCP, TCP_NODELAY, &nagle, sizeof(nagle));
    stream_.enable(ACE_NONBLOCK);
    (void)setMaximumSocketBufferSize(get_handle());

    const ACE_Reactor_Mask masks = ACE_Event_Handler::READ_MASK;
    if (reactor()->register_handler(this, masks) == -1) {
        NSRPC_LOG_ERROR2(
            ACE_TEXT("ACE_Reactor::register_handler() FAILED!!!(%d,%m)"), 
            ACE_OS::last_error());
        return false;
    }

    notifier_.reactor(reactor());
    msgQueue_.notification_strategy(&notifier_);
    assert(false == disconnectReserved_);
    assert(false == fireEventAfterFlush_);

    return true;
}


void ClientSession::disconnect_i(bool fireEvent)
{
    if (peer().get_handle() != ACE_INVALID_HANDLE) {
        const ACE_Reactor_Mask masks =
            ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
        reactor()->remove_handler(&notifier_, masks);
        reactor()->remove_handler(this, masks);
        disconnectReserved_ = false;
        fireEventAfterFlush_ = false;
        msgQueue_.flush();
        closeSocket();

        if (fireEvent) {
            onDisconnected();
        }
    }
}


void ClientSession::closeSocket()
{
    peer().close();
    peer().set_handle(ACE_INVALID_HANDLE);
}


bool ClientSession::read()
{
    if (recvBlock_->space() <= 0) {
        const size_t spare = recvBlock_->capacity() / 10;
        assert(spare > 0);
        recvBlock_->size(recvBlock_->size() + spare);
    }

    const ssize_t recvSize = peer().recv(recvBlock_->wr_ptr(),
        recvBlock_->space());
    if (recvSize < 0) {
        if (ACE_OS::last_error() == EWOULDBLOCK) {
            return true;
        }
        NSRPC_LOG_ERROR2(
            ACE_TEXT("ClientSession::read() FAILED!!!(%d,%m)"), 
            ACE_OS::last_error());
        return false;
    }
    else if (recvSize == 0) {
        NSRPC_LOG_DEBUG2(ACE_TEXT("Disconnected from server(%d,%m)."), 
            ACE_OS::last_error());
        return false;
    }

    recvBlock_->wr_ptr(recvSize);
    return true;
}


bool ClientSession::write()
{
    int queueSize = -1;

    {
        ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

        ACE_Message_Block* mblock;
        ACE_Time_Value immediate(ACE_Time_Value::zero);
        if (msgQueue_.dequeue_head(mblock, &immediate) == -1) {
            return true;
        }

        AceMessageBlockGuard block(mblock);

        const ssize_t sentSize =
            peer().send(block->rd_ptr(), block->length());
        if (sentSize == -1) {
            if (ACE_OS::last_error() != EWOULDBLOCK) {
                NSRPC_LOG_DEBUG2(
                    ACE_TEXT("ClientSession::write() ")
                    ACE_TEXT("FAILED!!!(%d,%m)"), ACE_OS::last_error());
                return false;
            }
        }
        else {
            block->rd_ptr(sentSize);
        }
        
        if (block->length() > 0) {
            const int result = msgQueue_.enqueue_head(block.get());
            if (result == -1) {
                NSRPC_LOG_ERROR2(
                    ACE_TEXT("ClientSession::write() - ")
                    ACE_TEXT("enqueue_head(%d, %m) FAILED!!!"),
                    ACE_OS::last_error());
                return false;
            }
            else {
                block.release(); // success
            }
        }

        queueSize = getWriteQueueSize();
    }

    if (queueSize >= 0) {
        NSRPC_LOG_INFO2("ClientSession: current write queue size = %u.",
            queueSize);
    }

    return true;
}


void ClientSession::releaseMessageBlocks()
{
    ACE_Message_Block* mblock;
    ACE_Time_Value immediate(ACE_Time_Value::zero);
    while (! msgQueue_.is_empty()) {
        if (msgQueue_.dequeue_head(mblock) != -1) {
            mblock->release();
        }
        else {
            assert(false);
            break;
        }
    }

    recvBlock_->release();
    msgBlock_->release();
}


bool ClientSession::parseHeader()
{
    assert(recvBlock_->length() >= packetCoder_->getHeaderSize());
    return packetCoder_->readHeader(headerForReceive_, *recvBlock_);
}


bool ClientSession::parseMessage()
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


bool ClientSession::isPacketHeaderArrived() const
{
    return recvBlock_->length() >= packetCoder_->getHeaderSize();
}


bool ClientSession::isMessageArrived() const
{
    return recvBlock_->length() >=
        (packetCoder_->getHeaderSize() + headerForReceive_.bodySize_);
}


int ClientSession::getWriteQueueSize()
{
    const time_t logInterval = 3;
    const size_t queueThreshold = 3;

    const time_t currentTime = time(0);
    if ((currentTime - lastLogTime_) >= logInterval) {
        const size_t currentQueueSize = msgQueue_.message_count();
        const size_t queueSizeDiff =
            (currentQueueSize > prevQueueSize_) ?
            currentQueueSize : prevQueueSize_;
        if (queueSizeDiff > 0) {
            lastLogTime_ = currentTime;
            if (queueSizeDiff > queueThreshold) {
                prevQueueSize_ = currentQueueSize;
                return static_cast<int>(currentQueueSize);
            }
        }
    }
    return -1;
}

// = ACE_Event_Handler overriding

int ClientSession::handle_input(ACE_HANDLE)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, -1);

    if (! read()) {
        return -1;
    }

    while (isPacketHeaderArrived()) {
        if (! parseHeader()) {
            return -1;
        }

        if (! isMessageArrived()) {
            break;
        }

        if (! parseMessage()) {
            return -1;
        }

        if (! isValidCsMessageType(headerForReceive_.messageType_)) {
            NSRPC_LOG_ERROR(ACE_TEXT("ClientSession::handle_input() - ")
                ACE_TEXT("Invalid Message Type."));
            return false;
        }

        onMessageArrived(headerForReceive_.messageType_);
    }

    return 0;
}


int ClientSession::handle_output(ACE_HANDLE)
{
    if (! write()) {
        return -1;
    }

    if (! msgQueue_.is_empty()) {
        reactor()->schedule_wakeup(this, ACE_Event_Handler::WRITE_MASK);
    }
    else {
        reactor()->cancel_wakeup(this, ACE_Event_Handler::WRITE_MASK);

        if (disconnectReserved_) {
            ACE_OS::shutdown(stream_.get_handle(), ACE_SHUTDOWN_WRITE);
            disconnect(fireEventAfterFlush_);
        }
    }

    return 0;
}


int ClientSession::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
    disconnect(true);

    return 0;
}

// = MessageBlockProvider overriding

ACE_Message_Block& ClientSession::acquireSendBlock()
{
    ACE_Message_Block* mblock =
        messageBlockManager_->create(packetCoder_->getDefaultPacketSize());
    mblock->wr_ptr(packetCoder_->getHeaderSize());
    return *mblock;
}


ACE_Message_Block& ClientSession::acquireRecvBlock()
{
    return *msgBlock_;
}

} // namespace nsrpc
