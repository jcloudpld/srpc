#include "stdafx.h"
#include <nsrpc/PacketSeedExchanger.h>
#include <nsrpc/detail/SessionRpcNetwork.h>

namespace nsrpc
{

IMPLEMENT_SRPC_EVENT_DISPATCHER(PacketSeedExchanger);

void PacketSeedExchanger::initialize(PacketCoder* packetCoder,
    SessionRpcNetwork* rpcNetwork)
{
    assert(packetCoder != 0);
    assert(rpcNetwork != 0);

    packetCoder_ = packetCoder;

    srpc::RpcReceiver::setRpcNetwork(rpcNetwork);
    srpc::RpcForwarder::setRpcNetwork(rpcNetwork);
}

} // namespace nsrpc
