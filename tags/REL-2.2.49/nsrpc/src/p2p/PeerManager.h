#ifndef NSRPC_PEERMANAGER_H
#define NSRPC_PEERMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "Peer.h"
#include <nsrpc/p2p/Group.h>
#include <srpc/ContainerTypes.h>
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace detail
{

class PeerNetworkSender;
class PeerMessageHandler;
class GroupManager;

/** @addtogroup p2p
* @{
*/

/**
 * @class PeerManager
 * P2P Peer Manager
 */
class PeerManager : public boost::noncopyable
{
    typedef srpc::HashMap<PeerId, PeerPtr> Peers;
public:
    PeerManager(PeerNetworkSender& networkSender,
        PeerMessageHandler& messageHandler,
        const P2pConfig& p2pConfig, const GroupManager& groupManager);
    ~PeerManager();

    void addPeer(PeerId peerId, const Addresses& addresses,
        P2pOptions p2pOptions);
    void removePeerNextTime(PeerId peerId);

    void addRelayServer(const PeerAddress& address);

    void reset();

    void putOutgoingMessage(GroupId groupId,
        const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, P2pOptions p2pOptions);
    void putOutgoingMessage(PeerId peerId,
        const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, P2pOptions p2pOptions);

    void putIncomingMessage(const P2pPacketHeader& header,
        const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock);

    void sendOutgoingMessages();

    void handleIncomingMessages();

    void handleDisconnectedPeers();

    void setHost(PeerId peerId);

    const PeerPtr getPeer(PeerId peerId);
    const PeerPtr getPeer(PeerId peerId) const;

    bool isExists(PeerId peerId) const {
        if (isRelayServer(peerId)) {
            return ! relayServer_.isNull();
        }
        return peers_.find(peerId) != peers_.end();
    }

    bool isHostConnected() const {
        return ! host_.isNull();
    }

    bool isHostCandidate(const RPeerIds& hostPrecedence) const;

    bool isHostAlive() const;

    size_t getPeerCount() const {
        return peers_.size();
    }

    PeerPtr getMe() const {
        return me_;
    }

    PeerPtr getRelayServer() const {
        return relayServer_;
    }

    bool isHost() const {
        return (! me_.isNull()) && (isHost(me_->getPeerId()));
    }

    bool isHost(PeerId peerId) const {
        return (! host_.isNull()) && (peerId == host_->getPeerId());
    }

    bool isSelf(PeerId peerId) const {
        return (! me_.isNull()) && (peerId == me_->getPeerId());
    }

    bool isHostExists() const {
        return ! host_.isNull();
    }

    PeerId getHostPeerId() const {
        return host_.isNull() ? invalidPeerId : host_->getPeerId();
    }

    bool canSendAcknowledgement(PeerId peerId) const {
        const PeerPtr peer(getPeer(peerId));
        return (! peer.isNull()) && (! peer->isDisconnecting());
    }
private:
    void putUnicastOutgoingMessage(PeerId peerId,
        const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, P2pOptions p2pOptions);
    void putBroadcastOutgoingMessage(const ACE_INET_Addr& toAddress,
        srpc::RpcPacketType packetType, ACE_Message_Block* mblock,
        P2pOptions p2pOptions);

    PeerPtr makePeer(PeerId peerId, const Addresses& addresses,
        P2pOptions p2pOptions);
    void removePeer(PeerId peerId);
private:
    PeerNetworkSender& networkSender_;
    PeerMessageHandler& messageHandler_;
    const P2pConfig& p2pConfig_;
    const GroupManager& groupManager_;

    Peers peers_;
    PeerPtr me_;
    PeerPtr host_;
    PeerPtr relayServer_;

    PeerIds disconnectedPeers_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERMANAGER_H
