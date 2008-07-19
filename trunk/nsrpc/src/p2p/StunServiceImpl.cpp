#include "stdafx.h"
#include "StunServiceImpl.h"
#include "StunServiceHandler.h"
#include <nsrpc/p2p/detail/PeerTime.h>
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/utility/LogManager.h>
#include <nsrpc/utility/Logger.h>
#include <cassert>

namespace nsrpc
{

namespace detail
{

// = RpcStunServiceImpl

IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcStunServiceImpl);

RpcStunServiceImpl::RpcStunServiceImpl(StunServiceHandler& serviceHandler,
    srpc::RpcNetwork& rpcNetwork) :
    srpc::RpcReceiver(&rpcNetwork),
    srpc::RpcForwarder(&rpcNetwork),
    serviceHandler_(serviceHandler),
    lastResolvingTime_(0),
    lastResolvedTime_(0)
{
}


void RpcStunServiceImpl::relayServerSetUp()
{
    lastResolvingTime_ = 0;
    lastResolvedTime_ = 0;
}


void RpcStunServiceImpl::resolve()
{
    const PeerTime currentTime = getPeerTime();

    if ((lastResolvingTime_ != 0) &&
        ((currentTime - lastResolvingTime_) <
            P2pConfig::minimalAddressResolvingInterval)) {
        return;
    }

    if ((lastResolvedTime_ != 0) &&
        ((currentTime - lastResolvedTime_) <
            P2pConfig::defaultNatPortHoldingInterval)) {
        return;
    }

    const P2pPeerHint hint(relayServerPeerId);
    rpcResolve(&hint);
    lastResolvingTime_ = currentTime;
}

// = RPC

DEFINE_SRPC_P2P_METHOD_0(RpcStunServiceImpl, rpcResolve, srpc::ptUnreliable);

IMPLEMENT_SRPC_P2P_METHOD_2(RpcStunServiceImpl, rpcResolved,
    srpc::RShortString, ipAddress, srpc::RUInt16, port, srpc::ptUnreliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());

    if (LogManager::isEnabledMask(LM_DEBUG)) {
        const ACE_INET_Addr fromAddress(hint.getAddress());
        NSRPC_LOG_DEBUG("rpcResolved()");
    }

    lastResolvedTime_ = getPeerTime();

    serviceHandler_.resolved(ipAddress, port);
}

} // namespace detail

} // namespace nsrpc
