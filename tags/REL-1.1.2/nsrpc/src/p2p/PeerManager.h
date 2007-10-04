#ifndef NSRPC_PEERMANAGER_H
#define NSRPC_PEERMANAGER_H

#include "Peer.h"
#include <boost/noncopyable.hpp>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <hash_map>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace nsrpc
{

namespace detail
{

class PeerNetworkSender;
class PeerMessageHandler;

/** @addtogroup p2p
* @{
*/

/**
 * @class PeerManager
 * P2P Peer Manager
 */
class PeerManager : public boost::noncopyable
{
    typedef stdext::hash_map<PeerId, PeerPtr> Peers;
public:
    PeerManager(PeerNetworkSender& networkSender,
        PeerMessageHandler& messageHandler,
        const P2pConfig& p2pConfig);
    ~PeerManager();

    void addPeer(PeerId peerId, const Addresses& addresses);
    void removePeerNextTime(PeerId peerId) {
        if (std::find(disconnectedPeers_.begin(), disconnectedPeers_.end(),
            peerId) == disconnectedPeers_.end()) {
            disconnectedPeers_.push_back(peerId);
        }
    }

    void addRelayServer(const PeerAddress& address);

    void reset();

    void putOutgoingMessage(PeerId peerId,
        const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock);

    void putIncomingMessage(const P2pPacketHeader& header,
        const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock);

    void sendOutgoingMessages(PeerTime currentTime);

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

    /// PeerId가 가장 작을 경우 호스트 후보자가 된다.
    bool isHostCandidate() const;

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
private:
    PeerPtr makePeer(PeerId peerId, const Addresses& addresses);
    void removePeer(PeerId peerId);
private:
    PeerNetworkSender& networkSender_;
    PeerMessageHandler& messageHandler_;
    const P2pConfig& p2pConfig_;

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
