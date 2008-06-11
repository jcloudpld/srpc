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
    neededSize_(0),
    disconnectReserved_(false),
    fireEventAfterFlush_(false)
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
        messageBlockManager_->create(packetCoder_->getDefaultPacketSize());
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
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, false);

    if (isConnected()) {
        assert(false && "Already connected. Disconnect first.");
        return false;
    }

    const ACE_INET_Addr address(port, ip.c_str());
    const ACE_Time_Value connectionTimeout(makeTimeValue(timeout));

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

    prepareRead();

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


void ClientSession::prepareRead()
{
    recvBlock_->reset();
    neededSize_ = packetCoder_->getHeaderSize();
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
    if (neededSize_ <= 0) {
        //assert(false && "neededSize_ <= 0");
        return false;
    }

    const ssize_t recvSize = peer().recv(recvBlock_->wr_ptr(), neededSize_);
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
    assert(neededSize_ >= static_cast<size_t>(recvSize));
    neededSize_ -= recvSize;
    return true;
}


bool ClientSession::write()
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
}


bool ClientSession::parseHeader()
{
    if (! packetCoder_->readHeader(headerForReceive_, *recvBlock_)) {
        return false;
    }

    neededSize_ = headerForReceive_.bodySize_;
    if (neededSize_ <= 0) {
        assert(neededSize_ > 0);
        return false;
    }

    if (recvBlock_->space() < neededSize_) {
        recvBlock_->size(recvBlock_->size() + neededSize_);
    }
    return true;
}


bool ClientSession::parseMessage()
{
    if (isConnected() && packetCoder_->decode(*recvBlock_)) {
        packetCoder_->advanceToBody(*recvBlock_);
        return true;
    }

    return false;
}


// 하나의 완전한 메세지 단위로 읽어들인 후 처리한다.
int ClientSession::handle_input(ACE_HANDLE)
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, lock_, -1);

    if (! read()) {
        return -1;
    }

    if (isMoreDataNeeded()) {
        return 0;
    }

    if (recvBlock_->length() == packetCoder_->getHeaderSize()) {
        if (! parseHeader()) {
            return -1;
        }
    }

    if (! isMoreDataNeeded()) {
        if (! parseMessage()) {
            return -1;
        }

        assert(isValidCsMessageType(headerForReceive_.messageType_));
        onMessageArrived(headerForReceive_.messageType_);

        prepareRead();
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


ACE_Message_Block& ClientSession::acquireSendBlock()
{
    ACE_Message_Block* mblock =
        messageBlockManager_->create(packetCoder_->getDefaultPacketSize());
    mblock->wr_ptr(packetCoder_->getHeaderSize());
    return *mblock;
}


ACE_Message_Block& ClientSession::acquireRecvBlock()
{
    return *recvBlock_;
}

} // namespace nsrpc
