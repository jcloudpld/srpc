#include "stdafx.h"
#include <nsrpc/RpcClientSession.h>
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/PacketSeedExchangerFactory.h>
#include <nsrpc/PacketSeedExchanger.h>

namespace nsrpc
{

// = RpcClientSession

IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcClientSession);

RpcClientSession::RpcClientSession(ACE_Reactor* reactor,
    PacketCoderFactory* packetCoderFactory, bool useBitPacking) :
    ClientSession(reactor, packetCoderFactory),
    rpcNetwork_(new SessionRpcNetwork(useBitPacking)),
    seedExchanger_(PacketSeedExchangerFactory::createForClient())
{
    initRpcNetwork();

    srpc::RpcReceiver::setRpcNetwork(rpcNetwork_.get());
    srpc::RpcForwarder::setRpcNetwork(rpcNetwork_.get());

    seedExchanger_->initialize(&getPacketCoder(), rpcNetwork_.get());
}


RpcClientSession::~RpcClientSession()
{
}


SessionRpcNetwork* RpcClientSession::getRpcNetwork()
{
    return rpcNetwork_.get();
}


void RpcClientSession::initRpcNetwork()
{
    rpcNetwork_->initialize(this, this);
}


void RpcClientSession::sendingFailed()
{
    onSendError();
    disconnect(true);
}


void RpcClientSession::receivingFailed()
{
    onReceiveError();
    disconnect(true);
}


void RpcClientSession::onConnected()
{
    initRpcNetwork();
}


void RpcClientSession::onDisconnected()
{
    rpcNetwork_->disable();
}


void RpcClientSession::onMessageArrived(CsMessageType messageType)
{
    if (! rpcNetwork_->messageArrived(messageType)) {
        receivingFailed();
    }
}

} // namespace nsrpc
