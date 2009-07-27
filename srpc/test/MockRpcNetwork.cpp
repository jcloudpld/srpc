#include "stdafx.h"
#include "MockRpcNetwork.h"
#include <srpc/StreamFactory.h>

MockRpcNetwork::MockRpcNetwork() :
    ostream_(
        srpc::StreamFactory::createOStream(true,
            srpc::StreamFactory::stBit, buffer_).release()),
    sendFailed_(false)
{
}


MockRpcNetwork::~MockRpcNetwork()
{
}


void MockRpcNetwork::send(srpc::RpcCommand& command,
    srpc::RpcPacketType /*packetType*/, const void* rpcHint)
{
    try {
        command.marshal(*ostream_);
        sentRpcHint_ = static_cast<int>(reinterpret_cast<size_t>(rpcHint));
        //sendFailed_ = false;
    }
    catch (const srpc::Exception&) {
        sendFailed_ = true;
    }
}
