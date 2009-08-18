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
    rpcNetwork_(config.rpcNetwork_),
    seedExchanger_(config.seedExchanger_)
{
    assert(rpcNetwork_.get() != 0);
    rpcNetwork_->initialize(*this, *this, config.shouldUseUtf8ForString_);

    seedExchanger_->initialize(*this, *config.packetCoder_, *rpcNetwork_);
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

RpcProactorSession::~RpcProactorSession()
{
}


void RpcProactorSession::registerRpcForwarder(srpc::RpcForwarder& forwarder)
{
    assert(rpcNetwork_.get() != 0);

    forwarder.setRpcNetwork(*rpcNetwork_);
}


void RpcProactorSession::registerRpcReceiver(srpc::RpcReceiver& receiver)
{
    assert(rpcNetwork_.get() != 0);

    receiver.setRpcNetwork(*rpcNetwork_);
}


srpc::RpcNetwork* RpcProactorSession::getRpcNetwork()
{
    return rpcNetwork_.get();
}


bool RpcProactorSession::onConnected()
{
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

    seedExchanger_->exchangeNextSeed();
    return true;
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
