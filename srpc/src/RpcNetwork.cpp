#include "stdafx.h"
#include <srpc/RpcNetwork.h>
#include <srpc/IStream.h>
#include <srpc/RpcReceiver.h>
#include <srpc/Exception.h>
#include <algorithm>

namespace srpc {

RpcNetwork::RpcNetwork()
{
    const size_t reservedCount = 10;

    // registerRpcReceiver 호출시 메모리 할당이 일어나는 것을 막기 위해
    receivers_.reserve(reservedCount);
}


void RpcNetwork::registerRpcReceiver(RpcReceiver* receiver)
{
    assert(receiver != 0);
    assert(std::find(receivers_.begin(), receivers_.end(), receiver) ==
        receivers_.end());
    receivers_.push_back(receiver);
}


void RpcNetwork::unregisterRpcReceiver(RpcReceiver* receiver)
{
    assert(receiver != 0);
    RpcReceivers::iterator pos =
        std::find(receivers_.begin(), receivers_.end(), receiver);
    if (pos != receivers_.end()) {
        receivers_.erase(pos);
    }
}


void RpcNetwork::onReceive(IStream& istream, const void* rpcHint)
{
    bool isHandled = false;
    RpcId rpcId;
    rpcId.read(istream);

    const RpcReceivers::const_iterator end = receivers_.end();
    for (RpcReceivers::const_iterator pos = receivers_.begin();
        pos != end; ++pos) {
        RpcReceiver* receiver = *pos;
        if (receiver->handle(rpcId, istream, rpcHint)) {
            isHandled = true;
            break;
        }
    }

    if (! isHandled) {
        std::ostringstream oss;
        oss << "RPC Id: " << rpcId;
        throw srpc::UnknownRpcMethodException(__FILE__, __LINE__,
            oss.str().c_str());
    }
}

} // namespace srpc
