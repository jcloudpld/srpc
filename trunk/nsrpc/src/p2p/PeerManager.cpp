#include "stdafx.h"
#include "PeerManager.h"
#include "PeerMessageHandler.h"
#include "GroupManager.h"

namespace nsrpc
{

namespace detail
{

PeerManager::PeerManager(PeerNetworkSender& networkSender,
    PeerMessageHandler& messageHandler, const P2pConfig& p2pConfig,
    const GroupManager& groupManager) :
    networkSender_(networkSender),
    messageHandler_(messageHandler),
    p2pConfig_(p2pConfig),
    groupManager_(groupManager)
{
}


PeerManager::~PeerManager()
{
}


void PeerManager::addPeer(PeerId peerId, const Addresses& addresses,
    P2pOptions p2pOptions)
{
    PeerPtr peer(getPeer(peerId));
    if (peer.isNull()) {
        peer = makePeer(peerId, addresses, p2pOptions);
        peers_.insert(Peers::value_type(peerId, peer));
    }

    if (me_.isNull()) {
        me_ = peer;
    }
}


void PeerManager::removePeerNextTime(PeerId peerId)
{
    const PeerIds::const_iterator pos = std::find(disconnectedPeers_.begin(),
        disconnectedPeers_.end(), peerId);
    if (pos == disconnectedPeers_.end()) {
        disconnectedPeers_.push_back(peerId);
    }
}


void PeerManager::removePeer(PeerId peerId)
{
    if (isExists(peerId)) {
        peers_.erase(peerId);
    }

    if (isHostExists() && (host_->getPeerId() == peerId)) {
        host_.reset();
    }

    if ((! me_.isNull()) && (me_->getPeerId() == peerId)) {
        me_.reset();
    }

    if ((! relayServer_.isNull()) && (relayServer_->getPeerId() == peerId)) {
        relayServer_.reset();
    }
}


void PeerManager::addRelayServer(const PeerAddress& address)
{
    assert(address.isValid());

    Addresses addresses;
    addresses.push_back(ACE_INET_Addr(address.port_, address.ip_.c_str()));

    if (! relayServer_.isNull()) {
        removePeer(relayServer_->getPeerId());
    }

    relayServer_ = makePeer(relayServerPeerId, addresses, poNone);
    relayServer_->connected();
}


PeerPtr PeerManager::makePeer(PeerId peerId, const Addresses& addresses,
    P2pOptions p2pOptions)
{
    assert(! isExists(peerId));
    return PeerPtr(new Peer(peerId, addresses, p2pOptions, networkSender_,
        messageHandler_, p2pConfig_));
}


void PeerManager::setHost(PeerId peerId)
{
    if (! isExists(peerId)) {
        assert(false);
        return;
    }

    if (isHostExists()) {
        // 이 어썰트가 유효하지 않음 (정확한 원인 파악 안됨)
        //assert(! isHost(peerId));
        if (! isHost(peerId)) {
            removePeerNextTime(host_->getPeerId());
        }
    }
    host_ = getPeer(peerId);
}


void PeerManager::reset()
{
    peers_.clear();
    host_.reset();
    me_.reset();
    relayServer_.reset();
    disconnectedPeers_.clear();
}


void PeerManager::putOutgoingMessage(GroupId groupId,
    const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
    ACE_Message_Block* mblock, P2pOptions p2pOptions)
{
    assert(isValid(groupId));

    const RPeerIds* peerIds = groupManager_.getGroupPeerIds(groupId);
    if (! peerIds) {
        assert(false && "why nobody exist?");
        return;
    }

    RPeerIds::const_iterator pos = peerIds->begin();
    const RPeerIds::const_iterator end = peerIds->end();
    for (; pos != end; ++pos) {
        const PeerId peerId = *pos;
        putUnicastOutgoingMessage(peerId, toAddress, packetType, mblock,
            p2pOptions);
    }
}


void PeerManager::putOutgoingMessage(PeerId peerId,
    const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
    ACE_Message_Block* mblock, P2pOptions p2pOptions)
{
    assert(! isSelf(peerId));

    if (isValidPeerId(peerId)) {
        putUnicastOutgoingMessage(peerId, toAddress, packetType, mblock,
            p2pOptions);
    }
    else {
        putBroadcastOutgoingMessage(toAddress, packetType, mblock, p2pOptions);
    }
}


void PeerManager::putUnicastOutgoingMessage(PeerId peerId,
    const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
    ACE_Message_Block* mblock, P2pOptions p2pOptions)
{
    const PeerPtr peer(getPeer(peerId));
    if (peer.isNull()) {
        assert(false);
        return;
    }

    if (peer->isAllowed(p2pOptions)) {
        peer->putOutgoingMessage(packetType, mblock, toAddress);
    }
}


void PeerManager::putBroadcastOutgoingMessage(const ACE_INET_Addr& toAddress,
    srpc::RpcPacketType packetType, ACE_Message_Block* mblock,
    P2pOptions p2pOptions)
{
    Peers::iterator pos = peers_.begin();
    const Peers::iterator end = peers_.end();
    for (; pos != end; ++pos) {
        const PeerPtr& peer = (*pos).second;
        if (isSelf(peer->getPeerId())) {
            continue;
        }
        if (peer->isAllowed(p2pOptions)) {
            peer->putOutgoingMessage(packetType, mblock, toAddress);
        }
    }
}


void PeerManager::putIncomingMessage(const P2pPacketHeader& header,
    const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock)
{
    assert(isExists(header.peerId_));
    assert(! isSelf(header.peerId_));

    const PeerPtr peer(getPeer(header.peerId_));
    peer->putIncomingMessage(header, mblock, peerAddress);
}


void PeerManager::sendOutgoingMessages()
{
    if (me_.isNull()) {
        return;
    }

    const PeerId myPeerId = me_->getPeerId();

    if (! relayServer_.isNull()) {
        relayServer_->sendOutgoingMessages(myPeerId);
    }

    Peers::iterator pos = peers_.begin();
    const Peers::iterator end = peers_.end();
    for (; pos != end; ++pos) {
        const PeerPtr& peer = (*pos).second;
        if (peer->getPeerId() == myPeerId) {
            continue;
        }

        peer->sendOutgoingMessages(myPeerId);

        if (peer->isDisconnecting()) {
            removePeerNextTime(peer->getPeerId());
        }
    }
}


void PeerManager::handleIncomingMessages()
{
    if (! relayServer_.isNull()) {
        const bool result = relayServer_->handleIncomingMessages();
        assert(result); result;
    }

    Peers::iterator pos = peers_.begin();
    const Peers::iterator end = peers_.end();
    for (; pos != end; ++pos) {
        const PeerPtr& peer = (*pos).second;
        if (isSelf(peer->getPeerId())) {
            continue;
        }

        if (! peer->handleIncomingMessages()) {
            removePeerNextTime(peer->getPeerId());
        }
        else if (peer->isAcknowledgingDisconnect()) {
            removePeerNextTime(peer->getPeerId());
        }
    }
}


void PeerManager::handleDisconnectedPeers()
{
    for (size_t i = 0; i < disconnectedPeers_.size(); ++i) {
        const PeerId peerId = disconnectedPeers_[i];
        messageHandler_.peerDisconnecting(peerId);
        removePeer(peerId);
    }
    disconnectedPeers_.clear();
}


const PeerPtr PeerManager::getPeer(PeerId peerId)
{
    if (isRelayServer(peerId)) {
        return relayServer_;
    }

    Peers::iterator pos = peers_.find(peerId);
    if (pos != peers_.end()) {
        return (*pos).second;
    }
    return PeerPtr();
}


const PeerPtr PeerManager::getPeer(PeerId peerId) const
{
    if (isRelayServer(peerId)) {
        return relayServer_;
    }

    const Peers::const_iterator pos = peers_.find(peerId);
    if (pos != peers_.end()) {
        return (*pos).second;
    }
    return PeerPtr();
}


bool PeerManager::isHostCandidate(const RPeerIds& hostPrecedence) const
{
    // 호스트가 아니고 hostPrecedence의 제일 앞부분에 있는 피어가 호스트 후보
    if (! hostPrecedence.empty()) {
        RPeerIds::const_iterator pos = hostPrecedence.begin();
        const RPeerIds::const_iterator end = hostPrecedence.end();
        for (; pos != end; ++pos) {
            const PeerId peerId = *pos;
            const PeerPtr peer(getPeer(peerId));
            if (peer.isNull()) {
                continue;
            }
            if (peer == host_) {
                continue;
            }
            return (peer == me_);
        }
    }

    /// PeerId가 가장 작을 경우 호스트 후보가 된다.
    Peers::const_iterator posPeers = peers_.begin();
    const Peers::const_iterator endPeers = peers_.end();
    for (; posPeers != endPeers; ++posPeers) {
        const PeerPtr& peer = (*posPeers).second;
        if (peer != host_) {
            if (peer->getPeerId() < me_->getPeerId()) {
                return false;
            }
        }
    }

    return true;
}


bool PeerManager::isHostAlive() const
{
    if (host_.isNull()) {
        return false;
    }

    if (host_ == me_) {
        return true;
    }

    return (getPeerTime() - host_->getLastReceiveTime()) <
        p2pConfig_.hostAliveConditionTime_;
}

} // namespace detail

} // namespace nsrpc
