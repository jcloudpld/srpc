#include "stdafx.h"
#include <nsrpc/PacketSeedExchanger.h>
#include <nsrpc/detail/SessionRpcNetwork.h>

namespace nsrpc
{

IMPLEMENT_SRPC_EVENT_DISPATCHER(PacketSeedExchanger);

void PacketSeedExchanger::initialize(
    PacketSeedExchangerCallback& callback,
    PacketCoder& packetCoder,
    SessionRpcNetwork& rpcNetwork)
{
    callback_ = &callback;
    packetCoder_ = &packetCoder;

    srpc::RpcReceiver::setRpcNetwork(rpcNetwork);
    srpc::RpcForwarder::setRpcNetwork(rpcNetwork);
}

} // namespace nsrpc
