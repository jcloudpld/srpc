#include "stdafx.h"
#include "PeerManager.h"
#include "PeerMessageHandler.h"

namespace nsrpc
{

namespace detail
{

PeerManager::PeerManager(PeerNetworkSender& networkSender,
    PeerMessageHandler& messageHandler, const P2pConfig& p2pConfig) :
    networkSender_(networkSender),
    messageHandler_(messageHandler),
    p2pConfig_(p2pConfig)
{
}



PeerManager::~PeerManager()
{
}


void PeerManager::addPeer(PeerId peerId, const Addresses& addresses)
{
    PeerPtr peer(getPeer(peerId));
    if (! isExists(peerId)) {
        peer = makePeer(peerId, addresses);
        peers_.insert(Peers::value_type(peerId, peer));
    }

    if (me_.isNull()) {
        me_ = peer;
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

    relayServer_ = makePeer(relayServerPeerId, addresses);
    relayServer_->connected();
}


PeerPtr PeerManager::makePeer(PeerId peerId, const Addresses& addresses)
{
    assert(! isExists(peerId));
    return PeerPtr(new Peer(peerId, addresses, networkSender_,
        messageHandler_, p2pConfig_));
}


void PeerManager::setHost(PeerId peerId)
{
    if (! isExists(peerId)) {
        assert(false);
        return;
    }

    if (isHostExists()) {
        assert(! isHost(peerId));
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
}


void PeerManager::putOutgoingMessage(PeerId peerId,
    const ACE_INET_Addr& toAddress, srpc::RpcPacketType packetType,
    ACE_Message_Block* mblock)
{
    assert(! isSelf(peerId));

    if (isValidPeerId(peerId)) {
        const PeerPtr peer(getPeer(peerId));
        if (! peer.isNull()) {
            peer->putOutgoingMessage(packetType, mblock, toAddress);
        }
        else {
            assert(false);
        }
    }
    else {
        Peers::iterator pos = peers_.begin();
        const Peers::iterator end = peers_.end();
        for (; pos != end; ++pos) {
            const PeerPtr& peer = (*pos).second;
            if (isSelf(peer->getPeerId())) {
                continue;
            }
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
    PeerIds::const_iterator pos = disconnectedPeers_.begin();
    const PeerIds::const_iterator end = disconnectedPeers_.end();
    for (; pos != end; ++pos) {
        const PeerId peerId = *pos;
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

    Peers::const_iterator pos = peers_.find(peerId);
    if (pos != peers_.end()) {
        return (*pos).second;
    }
    return PeerPtr();
}


bool PeerManager::isHostCandidate() const
{
    const PeerPtr me(getMe());

    Peers::const_iterator pos = peers_.begin();
    const Peers::const_iterator end = peers_.end();
    for (; pos != end; ++pos) {
        const PeerPtr& peer = (*pos).second;
        if (peer != host_) {
            if (peer->getPeerId() < me->getPeerId()) {
                return false;
            }
        }
    }
    return true;
}

} // namespace detail

} // namespace nsrpc
