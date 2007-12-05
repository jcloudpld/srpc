#include "stdafx.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcNetwork.h>

namespace srpc {

void RpcForwarder::forward(RpcCommand& command, RpcPacketType packetType,
    const void* rpcHint)
{
    if (rpcNetwork_ != 0) {
        rpcNetwork_->send(command, packetType, rpcHint);
    }
}

} // namespace srpc
