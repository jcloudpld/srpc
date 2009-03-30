#include "stdafx.h"
#include "SystemServiceImpl.h"
#include "SystemServiceHandler.h"
#include "PeerManager.h"
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/utility/LogManager.h>
#include <nsrpc/utility/Logger.h>
#include <cassert>

namespace nsrpc
{

namespace detail
{

IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcSystemServiceImpl);

RpcSystemServiceImpl::RpcSystemServiceImpl(PeerManager& peers,
    P2pEventHandler& eventHandler, SystemServiceHandler& serviceHandler,
    srpc::RpcNetwork& rpcNetwork) :
    srpc::RpcReceiver(&rpcNetwork),
    srpc::RpcForwarder(&rpcNetwork),
    peerManager_(peers),
    eventHandler_(eventHandler),
    serviceHandler_(serviceHandler)
{
    reset();
}


void RpcSystemServiceImpl::setHost(const RP2pProperty& p2pProperty)
{
    isHost_ = true;
    p2pProperty_ = p2pProperty;
}


void RpcSystemServiceImpl::reset()
{
    isHost_ = false;
    p2pProperty_.reset();
}


void RpcSystemServiceImpl::connectToNewPeer(const RPeerInfo& pi)
{
    assert(isValidPeerId(pi.peerId_));
    if ((! peerManager_.isHost(pi.peerId_)) &&
        (! peerManager_.isSelf(pi.peerId_))) {
        serviceHandler_.connectToNewPeer(pi.peerId_,
            toAddresses(pi.addresses_));
    }
}


void RpcSystemServiceImpl::reconnectToPeer(PeerId peerId,
    const RAddresses& peerAddresses)
{
    assert(! peerManager_.isSelf(peerId));
    assert(peerManager_.isExists(peerId));

    const PeerPtr peer(peerManager_.getPeer(peerId));

    const PeerPtr me(peerManager_.getMe());
    const RAddresses myAddresses = me->getPeerAddresses();

    RAddresses::const_iterator pos = peerAddresses.begin();
    const RAddresses::const_iterator end = peerAddresses.end();
    for (; pos != end; ++pos) {
        const ACE_INET_Addr& address = *pos;
        const P2pPeerHint hint(peerId, &address);
        for (int i = 0; i < P2pConfig::peerReconnectTryCount; ++i) {
            rpcConnectReversal(myAddresses, peer->getP2pOptions(), &hint);
        }

        NSRPC_LOG_DEBUG4(ACE_TEXT("Try to reconnect(P%u) via (%s:%d)"),
            peerId, address.get_host_addr(), address.get_port_number());
    }
}


// 처리 시간을 조금이라도 줄이기 위해서 우회함
void RpcSystemServiceImpl::logRpc(const char* rpc, const P2pPeerHint& hint,
    SequenceNumber sequenceNumber, PeerTime sentTime)
{
    if (! LogManager::isEnabledMask(LM_DEBUG)) {
        return;
    }

    const ACE_INET_Addr fromAddress(hint.getAddress());
    NSRPC_LOG_DEBUG8("%s(P%u->P%u, %s:%d, #%d, %d)",
        rpc, hint.peerId_, peerManager_.getMe()->getPeerId(),
        fromAddress.get_host_addr(), fromAddress.get_port_number(),
        sequenceNumber, sentTime);
}

// = RpcSystemService overriding

IMPLEMENT_SRPC_P2P_METHOD_4(RpcSystemServiceImpl, rpcConnect,
    RAddresses, peerAddresses, RP2pOptions, p2pOptions,
    srpc::RShortString, sessionPassword, srpc::RUInt32, sessionKey,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcConnect", hint);

    if (peerManager_.isSelf(hint.peerId_)) {
        return;
    }

    if (! serviceHandler_.authenticate(hint.peerId_, sessionPassword,
        sessionKey)) {
        return;
    }

    const ACE_INET_Addr fromAddress(hint.getAddress());

    if (isHost_) {
        if ((peerManager_.getPeerCount() >= 2) &&
            (! peerManager_.isExists(hint.peerId_))) {
            rpcNewPeerConnected(RPeerInfo(hint.peerId_, peerAddresses,
                p2pOptions));
        }
    }
    else {
        if (! serviceHandler_.isHostConnected()) {
            NSRPC_LOG_DEBUG4(
                "I'm not a host, but P%u(%s:%d) tried to connect me.",
                hint.peerId_,
                fromAddress.get_host_addr(), fromAddress.get_port_number());
            return;
        }
    }

    if (! serviceHandler_.peerConnected(hint.peerId_, fromAddress,
        peerAddresses, p2pOptions)) {
        return;
    }

    const PeerPtr peer(peerManager_.getPeer(hint.peerId_));
    assert(! peer.isNull());
    peer->acknowledgingConnect();

    const PeerPtr me(peerManager_.getMe());
    rpcConnected(me->getPeerAddresses(), isHost_, p2pProperty_, p2pOptions,
        serviceHandler_.getCurrentGroups(), &hint);
}


IMPLEMENT_SRPC_P2P_METHOD_5(RpcSystemServiceImpl, rpcConnected,
    RAddresses, peerAddresses, srpc::RBool, isHost, RP2pProperty, p2pProperty,
    RP2pOptions, p2pOptions,
    RGroupMap, groups,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcConnected", hint);

    (void)serviceHandler_.peerConnected(hint.peerId_, hint.getAddress(),
        peerAddresses, p2pOptions);

    const PeerPtr peer(peerManager_.getPeer(hint.peerId_));
    assert(! peer.isNull());
    if (peer->isDisconnected()) {
        peer->connected(true);
        eventHandler_.onPeerConnected(hint.peerId_);
    }

    if (isHost) {
        peerManager_.setHost(hint.peerId_);
        serviceHandler_.setP2pProperty(p2pProperty);
        p2pProperty_ = p2pProperty;
        serviceHandler_.setGroups(groups);
    }

    if (serviceHandler_.shouldConnectReversal(hint.getAddress())) {
        rpcRequestConnectReversal(peerManager_.getMe()->getPeerAddresses(),
            &hint);
    }
}


IMPLEMENT_SRPC_P2P_METHOD_1(RpcSystemServiceImpl, rpcRequestConnectReversal,
    RAddresses, peerAddresses, srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcRequestConnectReversal", hint);

    if (! peerManager_.isExists(hint.peerId_)) {
        assert(false);
        return;
    }

    if (serviceHandler_.shouldConnectReversal(hint.getAddress())) {
        reconnectToPeer(hint.peerId_, peerAddresses);
    }
}


IMPLEMENT_SRPC_P2P_METHOD_2(RpcSystemServiceImpl, rpcConnectReversal,
    RAddresses, peerAddresses, RP2pOptions, p2pOptions, srpc::ptUnreliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    assert(! isHost_);
    logRpc("rpcConnectReversal", hint);

    if (! peerManager_.isExists(hint.peerId_)) {
        assert(false);
        return;
    }

    (void)serviceHandler_.peerConnected(hint.peerId_, hint.getAddress(),
        peerAddresses, p2pOptions);

    const PeerPtr me(peerManager_.getMe());
    const PeerPtr peer(peerManager_.getPeer(hint.peerId_));
    rpcConnected(me->getPeerAddresses(), isHost_, p2pProperty_, p2pOptions,
        serviceHandler_.getCurrentGroups(), &hint);
}


IMPLEMENT_SRPC_P2P_METHOD_1(RpcSystemServiceImpl, rpcNewPeerConnected,
    RPeerInfo, peerInfo, srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcNewPeerConnected", hint);

    if (! peerManager_.isExists(hint.peerId_)) {
        return;
    }

    if (isHost_) {
        return;
    }

    connectToNewPeer(peerInfo);
}


IMPLEMENT_SRPC_P2P_METHOD_0(RpcSystemServiceImpl, rpcDisconnect,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcDisconnect", hint);

    if (! peerManager_.isExists(hint.peerId_)) {
        return;
    }

    if (peerManager_.isSelf(hint.peerId_)) {
        return;
    }

    const PeerPtr peer(peerManager_.getPeer(hint.peerId_));
    if (peer->isConnected()) {
        peer->acknowledgingDisconnect();
        serviceHandler_.peerDisconnected(peer->getPeerId());
    }
}


IMPLEMENT_SRPC_P2P_METHOD_0(RpcSystemServiceImpl, rpcPing, srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcPing", hint);

    // NOP
}


IMPLEMENT_SRPC_P2P_METHOD_2(RpcSystemServiceImpl, rpcAcknowledgement,
    RSequenceNumber, sequenceNumber, RRelativeTime, sentTime,
    srpc::ptUnreliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcAcknowledgement", hint, sequenceNumber, sentTime);

    const PeerPtr peer(peerManager_.getPeer(hint.peerId_));
    if (! peer.isNull()) {
        peer->acknowledged(sequenceNumber, sentTime);
    }
}


IMPLEMENT_SRPC_P2P_METHOD_0(RpcSystemServiceImpl, rpcHostMigrated,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcHostMigrated", hint);

    serviceHandler_.hostMigrated(hint.peerId_);
}


IMPLEMENT_SRPC_P2P_METHOD_1(RpcSystemServiceImpl, rpcGroupCreated,
    RGroupInfo, groupInfo,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcGroupCreated", hint);

    serviceHandler_.groupCreated(groupInfo);
}


IMPLEMENT_SRPC_P2P_METHOD_1(RpcSystemServiceImpl, rpcGroupDestroyed,
    RGroupId, groupId,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcGroupDestroyed", hint);

    serviceHandler_.groupDestroyed(groupId);
}


IMPLEMENT_SRPC_P2P_METHOD_2(RpcSystemServiceImpl, rpcGroupJoined,
    RGroupId, groupId, RPeerId, peerId,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcGroupJoined", hint);

    serviceHandler_.groupJoined(groupId, peerId);
}


IMPLEMENT_SRPC_P2P_METHOD_2(RpcSystemServiceImpl, rpcGroupLeft,
    RGroupId, groupId, RPeerId, peerId,
    srpc::ptReliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());
    logRpc("rpcGroupLeft", hint);

    serviceHandler_.groupLeft(groupId, peerId);
}

} // namespace detail

} // namespace nsrpc
