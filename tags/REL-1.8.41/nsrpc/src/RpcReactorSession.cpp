#include "stdafx.h"
#include <nsrpc/RpcReactorSession.h>
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/PacketSeedExchangerFactory.h>
#include <nsrpc/PacketSeedExchanger.h>
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>

namespace nsrpc
{

// = RpcReactorSession

RpcReactorSession::RpcReactorSession(ACE_Reactor* reactor,
    PacketCoderFactory* packetCoderFactory, bool useBitPacking,
    bool shouldUseUtf8ForString) :
    ReactorSession(reactor, packetCoderFactory),
    rpcNetwork_(new SessionRpcNetwork(useBitPacking)),
    seedExchanger_(PacketSeedExchangerFactory::createForClient())
{
    rpcNetwork_->initialize(*this, *this, shouldUseUtf8ForString);

    seedExchanger_->initialize(getPacketCoder(), *rpcNetwork_);
}


RpcReactorSession::~RpcReactorSession()
{
}


void RpcReactorSession::registerRpcForwarder(srpc::RpcForwarder& forwarder)
{
    assert(rpcNetwork_.get() != 0);

    forwarder.setRpcNetwork(*rpcNetwork_);
}


void RpcReactorSession::registerRpcReceiver(srpc::RpcReceiver& receiver)
{
    assert(rpcNetwork_.get() != 0);

    receiver.setRpcNetwork(*rpcNetwork_);
}


void RpcReactorSession::sendingFailed()
{
    onSendError();
    disconnect(true);
}


void RpcReactorSession::receivingFailed()
{
    onReceiveError();
    disconnect(true);
}


void RpcReactorSession::onConnected()
{
    rpcNetwork_->reset();

    seedExchanger_->exchangeFirstSeed();
}


void RpcReactorSession::onDisconnected()
{
    rpcNetwork_->disable();
}


void RpcReactorSession::onMessageArrived(CsMessageType messageType)
{
    if (! rpcNetwork_->messageArrived(messageType)) {
        receivingFailed();
    }
}

} // namespace nsrpc
