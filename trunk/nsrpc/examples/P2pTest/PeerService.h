#ifndef P2PTEST_PEERSERVICE_H
#define P2PTEST_PEERSERVICE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <nsrpc/p2p/PeerId.h>
#include <srpc/RpcP2p.h>

/**
 * @class RpcPeerService
 * P2P Test RPC interface
 */
class RpcPeerService
{
public:
    virtual ~RpcPeerService() {}

    /// chat. reliable message.
    DECLARE_SRPC_METHOD_1(RpcPeerService, chat,
        srpc::RShortString, message);

    /// tick. reliable message.
    DECLARE_SRPC_METHOD_1(RpcPeerService, tick,
        srpc::RUInt32, tick);

    /// dummy. unreliable message
    DECLARE_SRPC_METHOD_0(RpcPeerService, dummy);
};

#endif // P2PTEST_PEERSERVICE_H
