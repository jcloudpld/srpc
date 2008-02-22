#include "stdafx.h"
#include "PeerCandidate.h"
#include <nsrpc/p2p/detail/PeerNetworkSender.h>

namespace nsrpc
{

namespace detail
{

/**
 * @class PeerCandidate
 */
class PeerCandidate : public SharedObject
{
public:
    PeerCandidate(PeerId peerId, const AddressPair& addressPair,
        PeerNetworkSender& networkSender, const P2pConfig& p2pConfig) :
        peerId_(peerId),
        addressPair_(addressPair),
        networkSender_(networkSender),
        p2pConfig_(p2pConfig),
        startTime_(getPeerTime()),
        sentTime_(0) {}

    void setOutgoingMessage(PeerId peerId, const ACE_INET_Addr& peerAddress,
        ACE_Message_Block* mblock) {
        assert(! peerAddress.is_any());
        assert(! outgoingMessage_.mblock_);

        outgoingMessage_ = UnknownReliableMessage(firstSequenceNumber,
            mblock->clone(), peerAddress, getPeerTime(), peerId);
    }

    void sendOutgoingMessage() {
        assert(outgoingMessage_.peerAddress_ == addressPair_.targetAddress_);
        assert(outgoingMessage_.mblock_ != 0);

        if (networkSender_.sendNow(
            PeerIdPair(outgoingMessage_.peerId_, peerId_),
            addressPair_, *outgoingMessage_.mblock_, srpc::ptReliable,
            outgoingMessage_.sequenceNumber_, outgoingMessage_.sentTime_)) {
            sentTime_ = getPeerTime();
        }
    }

    PeerId getPeerId() const {
        return peerId_;
    }

    const ACE_INET_Addr& getTargetAddress() const {
        return addressPair_.targetAddress_;
    }

    bool isConnectionTimeout(PeerTime currentTime) const {
        return (currentTime - startTime_) >=
            p2pConfig_.connectTimeout_;
    }

    bool shouldRetransmit(PeerTime currentTime) const {
        return (sentTime_ == 0) ||
            ((currentTime - sentTime_) >= p2pConfig_.defaultRtt_);
    }
protected: // for SmartPtr
    ~PeerCandidate() {
        outgoingMessage_.release();
    }
private:
    bool shouldRelay() const {
        return ! addressPair_.isSame();
    }
private:
    const PeerId peerId_;
    const AddressPair addressPair_;
    PeerNetworkSender& networkSender_;
    const P2pConfig& p2pConfig_;

    const PeerTime startTime_;
    PeerTime sentTime_;

    UnknownReliableMessage outgoingMessage_;
};

// = PeerCandidateManager

PeerCandidateManager::PeerCandidateManager(PeerNetworkSender& networkSender,
    const P2pConfig& p2pConfig, PeerId myPeerId) :
    networkSender_(networkSender),
    p2pConfig_(p2pConfig),
    myPeerId_(myPeerId)
{
}


PeerCandidateManager::~PeerCandidateManager()
{
}


void PeerCandidateManager::reset()
{
    candidates_.clear();
}


void PeerCandidateManager::addCandidate(PeerId peerId,
    const AddressPair& addressPair)
{
    if (isExists(getCandidatePos(peerId, addressPair.targetAddress_))) {
        return;
    }

    candidates_.push_back(PeerCandidatePtr(
        new PeerCandidate(peerId, addressPair, networkSender_, p2pConfig_)));
}


void PeerCandidateManager::removeCandidates(PeerId peerId)
{
    PeerCandidates::iterator pos = candidates_.begin();
    PeerCandidates::iterator end = candidates_.end();
    for (; pos != end;) {
        const PeerCandidatePtr& candidate = *pos;
        if (candidate->getPeerId() == peerId) {
            pos = candidates_.erase(pos);
            end = candidates_.end();
        }
        else {
            ++pos;
        }
    }
}


bool PeerCandidateManager::putOutgoingMessage(PeerId peerId,
    const ACE_INET_Addr& targetAddress, ACE_Message_Block* mblock)
{
    PeerCandidates::iterator pos = getCandidatePos(peerId, targetAddress);
    if (isExists(pos)) {
        const PeerCandidatePtr& candidate = *pos;
        candidate->setOutgoingMessage(myPeerId_, targetAddress, mblock);
        return true;
    }

    return false;
}


PeerIds PeerCandidateManager::detectConnectionTimeout()
{
    const PeerTime currentTime = getPeerTime();

    PeerIds failedPeerIds;
    failedPeerIds.reserve(candidates_.size());

    PeerCandidates::iterator pos = candidates_.begin();
    PeerCandidates::iterator end = candidates_.end();
    for (; pos != end;) {
        const PeerCandidatePtr& candidate = *pos;
        if (candidate->isConnectionTimeout(currentTime)) {
            const PeerId peerId = candidate->getPeerId();
            pos = candidates_.erase(pos);
            end = candidates_.end();
            if (! isExists(peerId)) {
                failedPeerIds.push_back(peerId);
            }
        }
        else {
            if (candidate->shouldRetransmit(currentTime)) {
                candidate->sendOutgoingMessage();
            }
            ++pos;
        }
    }
    return failedPeerIds;
}


bool PeerCandidateManager::isExists(PeerId peerId) const
{
    PeerCandidates::const_iterator pos = candidates_.begin();
    const PeerCandidates::const_iterator end = candidates_.end();
    for (; pos != end; ++pos) {
        const PeerCandidatePtr& candidate = *pos;
        if (candidate->getPeerId() == peerId) {
            return true;
        }
    }
    return false;
}


PeerCandidateManager::PeerCandidates::const_iterator
PeerCandidateManager::getCandidatePos(PeerId peerId,
    const ACE_INET_Addr& targetAddress) const
{
    PeerCandidates::const_iterator pos = candidates_.begin();
    const PeerCandidates::const_iterator end = candidates_.end();
    for (; pos != end; ++pos) {
        const PeerCandidatePtr& candidate = *pos;
        if ((candidate->getPeerId() == peerId) &&
            (candidate->getTargetAddress() == targetAddress)) {
            return pos;
        }
    }
    return candidates_.end();
}


PeerCandidateManager::PeerCandidates::iterator
PeerCandidateManager::getCandidatePos(PeerId peerId,
    const ACE_INET_Addr& targetAddress)
{
    PeerCandidates::iterator pos = candidates_.begin();
    const PeerCandidates::iterator end = candidates_.end();
    for (; pos != end; ++pos) {
        const PeerCandidatePtr& candidate = *pos;
        if ((candidate->getPeerId() == peerId) &&
            (candidate->getTargetAddress() == targetAddress)) {
            return pos;
        }
    }
    return candidates_.end();
}

} // namespace detail

} // namespace nsrpc
