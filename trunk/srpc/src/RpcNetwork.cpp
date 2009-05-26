#include "stdafx.h"
#include <srpc/RpcNetwork.h>
#include <srpc/IStream.h>
#include <srpc/RpcReceiver.h>
#include <srpc/Exception.h>
#include <srpc/utility/CUtils.h>
#include <algorithm>

namespace srpc {

namespace {

template <typename Exception>
inline void throwRpcException(const char* file, int fileno, RpcId rpcId)
{
    char msg[64];
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4996)
#endif
    snprintf(msg, sizeof(msg) - 1, "RPC Id: %u", rpcId);
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

    throw Exception(file, fileno, msg);
}

} // namespace

// = RpcNetwork

void RpcNetwork::registerRpcReceiver(RpcReceiver* receiver)
{
    assert(receiver != 0);

    const RpcIds rpcIds = receiver->getRegisteredRpcIds();

    RpcIds::const_iterator pos = rpcIds.begin();
    const RpcIds::const_iterator end = rpcIds.end();
    for (; pos != end; ++pos) {
        const RpcId rpcId = *pos;

        if (receivers_.find(rpcId) != receivers_.end()) {
            throwRpcException<DuplicatedRpcMethodException>(
                __FILE__, __LINE__, rpcId);
        }

        receivers_.insert(RpcReceivers::value_type(rpcId, receiver));
    }
}


void RpcNetwork::unregisterRpcReceiver(RpcReceiver* receiver)
{
    assert(receiver != 0);

    const RpcIds rpcIds = receiver->getRegisteredRpcIds();

    RpcIds::const_iterator pos = rpcIds.begin();
    const RpcIds::const_iterator end = rpcIds.end();
    for (; pos != end; ++pos) {
        const RpcId rpcId = *pos;

        receivers_.erase(rpcId);
    }
}


void RpcNetwork::onReceive(IStream& istream, const void* rpcHint)
{
    RRpcId rpcId;
    rpcId.read(istream);

    if (! handleMessage(rpcId, istream, rpcHint)) {
        throwRpcException<UnknownRpcMethodException>(
            __FILE__, __LINE__, rpcId);
    }
}


bool RpcNetwork::handleMessage(RpcId rpcId, IStream& istream,
    const void* rpcHint)
{
    const RpcReceivers::iterator pos = receivers_.find(rpcId);
    if (pos == receivers_.end()) {
        return false;
    }
    RpcReceiver* receiver = (*pos).second;

    return receiver->handle(rpcId, istream, rpcHint);
}

} // namespace srpc
