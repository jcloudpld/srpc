#include "stdafx.h"
#include <srpc/RpcNetwork.h>
#include <srpc/IStream.h>
#include <srpc/RpcReceiver.h>
#include <srpc/Exception.h>
#include <srpc/utility/CUtils.h>
#include <algorithm>

namespace srpc {

RpcNetwork::RpcNetwork()
{
    const size_t reservedCount = 10;

    // registerRpcReceiver 호출시 메모리 할당이 일어나는 것을 막기 위해
    receivers_.reserve(reservedCount);
}


void RpcNetwork::registerRpcReceiver(RpcReceiver& receiver)
{
    assert(std::find(receivers_.begin(), receivers_.end(), &receiver) ==
        receivers_.end());
    receivers_.push_back(&receiver);
}


void RpcNetwork::unregisterRpcReceiver(RpcReceiver& receiver)
{
    const RpcReceivers::iterator pos =
        std::find(receivers_.begin(), receivers_.end(), &receiver);
    if (pos != receivers_.end()) {
        receivers_.erase(pos);
    }
}


void RpcNetwork::onReceive(IStream& istream, const void* rpcHint)
{
    RRpcId rpcId;
    rpcId.read(istream);

    if (handleMessage(rpcId, istream, rpcHint)) {
        return; // succeeded
    }

    char msg[64];
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4996)
#endif
    snprintf(msg, sizeof(msg) - 1, "RPC Id: %u", rpcId.get());
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

    throw srpc::UnknownRpcMethodException(__FILE__, __LINE__, msg);
}


bool RpcNetwork::handleMessage(RpcId rpcId, IStream& istream,
    const void* rpcHint)
{
    RpcReceivers::const_iterator pos = receivers_.begin();
    const RpcReceivers::const_iterator end = receivers_.end();
    for (; pos != end; ++pos) {
        RpcReceiver* receiver = *pos;
        if (receiver->handle(rpcId, istream, rpcHint)) {
            return true;
        }
    }

    return false;
}

} // namespace srpc
