#include "stdafx.h"

#if defined (NSRPC_HAS_PROACTOR)

#include <nsrpc/RpcSession.h>
#include <nsrpc/RpcSessionConfig.h>
#include <nsrpc/PacketSeedExchanger.h>
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoder.h>

namespace nsrpc
{

// = RpcSession

IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcSession);

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

RpcSession::RpcSession(const RpcSessionConfig& config) :
    Session(config),
    rpcNetwork_(config.rpcNetwork_),
    seedExchanger_(config.seedExchanger_)
{
    assert(rpcNetwork_.get() != 0);

    rpcNetwork_->initialize(this, this);

    srpc::RpcReceiver::setRpcNetwork(getRpcNetwork());
    srpc::RpcForwarder::setRpcNetwork(getRpcNetwork());

    seedExchanger_->initialize(config.packetCoder_, rpcNetwork_.get());
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

RpcSession::~RpcSession()
{
}


void RpcSession::open(ACE_HANDLE new_handle, ACE_Message_Block& message_block)
{
    rpcNetwork_->reset();

    Session::open(new_handle, message_block);

    if (isConnected()) {
        ACE_GUARD(ACE_Thread_Mutex, recvMonitor, getRecvLock());
        ACE_GUARD(ACE_Thread_Mutex, sendMonitor, getSendLock());

        seedExchanger_->exchangeFirstSeed();
    }
}


srpc::RpcNetwork* RpcSession::getRpcNetwork()
{
    return rpcNetwork_.get();
}


void RpcSession::onDisconnected()
{
    rpcNetwork_->disable();
}


bool RpcSession::onMessageArrived(CsMessageType messageType)
{
    if (! rpcNetwork_->messageArrived(messageType)) {
        return false;
    }

    seedExchanger_->exchangeNextSeed();
    return true;
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
