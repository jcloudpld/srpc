#include "stdafx.h"
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/MessageBlockStreamBuffer.h>
#include <nsrpc/detail/SessionRpcNetworkCallback.h>
#include <nsrpc/detail/MessageBlockProvider.h>
#include <nsrpc/detail/SessionRpcHint.h>
#include <nsrpc/utility/Logger.h>
#include <srpc/detail/RpcCommand.h>
#include <srpc/StreamFactory.h>
#include <ace/Lock_Adapter_T.h>

namespace
{

inline srpc::StreamFactory::StreamType getStreamType(bool useBitPacking)
{
    return useBitPacking ?
        srpc::StreamFactory::stBit : srpc::StreamFactory::stByte;
}

} // namespace

namespace nsrpc
{

SessionRpcNetwork::SessionRpcNetwork(bool useBitPacking) :
    callback_(0),
    messageBlockProvider_(0),
    enabled_(false),
    useBitPacking_(useBitPacking),
    shouldUseUtf8ForString_(true),
    lock_(0)
{
}


SessionRpcNetwork::~SessionRpcNetwork()
{
}


void SessionRpcNetwork::initialize(SessionRpcNetworkCallback& callback,
    MessageBlockProvider& messageBlockProvider, bool shouldUseUtf8ForString,
    ACE_Recursive_Thread_Mutex* lock)
{
    callback_ = &callback;
    messageBlockProvider_ = &messageBlockProvider;
    shouldUseUtf8ForString_ = shouldUseUtf8ForString;

    if (lock != 0) {
        lock_.reset(new ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>(*lock));
    }
    else {
        lock_.reset(new ACE_Lock_Adapter<ACE_Null_Mutex>);
    }

    reset();
}


void SessionRpcNetwork::reset()
{
    assert(lock_ != 0);

    ACE_GUARD(ACE_Lock, monitor, *lock_);

    initInputStream(*getRecvBlock());

    enabled_ = true;
}


bool SessionRpcNetwork::messageArrived(CsMessageType /*messageType*/)
{
    return handleMessageNow(*getRecvBlock());
}


bool SessionRpcNetwork::handleMessageNow(ACE_Message_Block& mblock)
{
    assert(lock_ != 0);

    // 한번에 하나의 쓰레드에서만 호출하므로 lock이 필요 없다
    //ACE_GUARD_RETURN(ACE_Lock, monitor, *lock_, false);

    if (! enabled_) {
        return true;
    }

    initInputStream(mblock);

    try {
        onReceive(*istream_);
    }
    catch(const srpc::Exception& e) {
        NSRPC_LOG_ERROR2(
            ACE_TEXT("SessionRpcNetwork::handleMessageNow() FAILED(%s)."),
            e.what());
        return false;
    }
    return true;
}


void SessionRpcNetwork::unmarshalingErrorOccurred()
{
    callback_->unmarshalingErrorOccurred();
}


ACE_Message_Block* SessionRpcNetwork::getRecvBlock()
{
    return &(messageBlockProvider_->acquireRecvBlock());
}


void SessionRpcNetwork::registerRpcReceiver(srpc::RpcReceiver& receiver)
{
    assert(lock_ != 0);

    ACE_GUARD(ACE_Lock, monitor, *lock_);

    srpc::RpcNetwork::registerRpcReceiver(receiver);
}


void SessionRpcNetwork::unregisterRpcReceiver(srpc::RpcReceiver& receiver)
{
    assert(lock_ != 0);

    ACE_GUARD(ACE_Lock, monitor, *lock_);

    srpc::RpcNetwork::unregisterRpcReceiver(receiver);
}


void SessionRpcNetwork::send(srpc::RpcCommand& command,
    srpc::RpcPacketType /*packetType*/, const void* rpcHint)
{
    const SessionRpcHint sessionRpcHint = toSessionRpcHint(rpcHint);
    assert(isValidCsMessageType(sessionRpcHint.messageType_));

    assert(lock_ != 0);

    ACE_GUARD(ACE_Lock, monitor, *lock_);

    ACE_Message_Block* wblock = 0;
    if (sessionRpcHint.isValidSendBlock()) {
        wblock = static_cast<ACE_Message_Block*>(
            sessionRpcHint.sendBlock_)->clone();
    }
    else {
        wblock = marshal(command);
    }
    if (! wblock) {
        return;
    }

    callback_->sendNow(*wblock, sessionRpcHint.messageType_);
}


ACE_Message_Block* SessionRpcNetwork::marshal(srpc::RpcCommand& command)
{
    if (! enabled_) {
        return 0;
    }

    ACE_Message_Block* wblock = initOutputStream();

    try {
        command.marshal(*ostream_);
    }
    catch (const srpc::Exception& e) {
        NSRPC_LOG_DEBUG2(ACE_TEXT("SessionRpcNetwork::marshal() - ")
            ACE_TEXT("RpcCommand::marshal() FAILED(%s)."),
            e.what());

        wblock->release();
        callback_->marshalingErrorOccurred();
        return 0;
    }

    return wblock;
}


void SessionRpcNetwork::initInputStream(ACE_Message_Block& mblock)
{
    if (! istream_.get()) { // lazy evaluation
        rstreamBuffer_.reset(new MessageBlockStreamBuffer(&mblock));
        istream_.reset(
            srpc::StreamFactory::createIStream(shouldUseUtf8ForString_,
                getStreamType(useBitPacking_), *rstreamBuffer_).release());
    }
    else {
        istream_->reset(false);
        rstreamBuffer_->reset(&mblock);
    }
}


ACE_Message_Block* SessionRpcNetwork::initOutputStream()
{
    ACE_Message_Block* wblock = &messageBlockProvider_->acquireSendBlock();
    if (! ostream_.get()) { // lazy evaluation
        wstreamBuffer_.reset(new MessageBlockStreamBuffer(wblock));
        ostream_.reset(
            srpc::StreamFactory::createOStream(shouldUseUtf8ForString_,
                getStreamType(useBitPacking_), *wstreamBuffer_).release());
    }
    else {
        ostream_->reset(false);
        wstreamBuffer_->reset(wblock);
    }
    return wblock;
}

} // namespace nsrpc
