#include "stdafx.h"

#if defined (NSRPC_HAS_PROACTOR)

#include <nsrpc/RpcProactorSession.h>
#include <nsrpc/RpcSessionConfig.h>
#include <nsrpc/PacketSeedExchanger.h>
#include <nsrpc/PacketSeedExchangerCallback.h>
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoder.h>
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>

namespace nsrpc
{

// = RpcProactorSession

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

RpcProactorSession::RpcProactorSession(const RpcSessionConfig& config) :
    ProactorSession(config),
    sessionConfig_(config),
    rpcNetwork_(config.rpcNetwork_),
    seedExchanger_(config.seedExchanger_),
    isInitialized_(false)
{
    assert(rpcNetwork_.get() != 0);

    initializeLazily();
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

RpcProactorSession::~RpcProactorSession()
{
}


void RpcProactorSession::registerRpcForwarder(srpc::RpcForwarder& forwarder)
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, monitor, getLock());

    initializeLazily();

    forwarder.setRpcNetwork(*rpcNetwork_);
}


void RpcProactorSession::registerRpcReceiver(srpc::RpcReceiver& receiver)
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, monitor, getLock());

    initializeLazily();

    receiver.setRpcNetwork(*rpcNetwork_);
}


srpc::RpcNetwork& RpcProactorSession::getRpcNetwork()
{
    return *rpcNetwork_;
}


void RpcProactorSession::initializeLazily()
{
    if (isInitialized_) {
        return;
    }

    rpcNetwork_->initialize(*this, *this,
        sessionConfig_.shouldUseUtf8ForString_, &getLock());
    seedExchanger_->initialize(*this, *sessionConfig_.packetCoder_,
        *rpcNetwork_);
    isInitialized_ = true;
    return;
}


bool RpcProactorSession::onConnected()
{
    initializeLazily();

    rpcNetwork_->reset();
    seedExchanger_->exchangePublicKey();
    return true;
}


void RpcProactorSession::onDisconnected()
{
    rpcNetwork_->disable();
}


bool RpcProactorSession::onMessageArrived(CsMessageType messageType)
{
    if (! rpcNetwork_->messageArrived(messageType)) {
        return false;
    }

    {
        ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, monitor, getLock(), false);

        seedExchanger_->exchangeNextSeed();
    }

    return true;
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
