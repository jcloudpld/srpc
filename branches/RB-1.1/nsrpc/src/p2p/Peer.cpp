#include "stdafx.h"
#include "Peer.h"
#include "PeerMessageHandler.h"
#include <nsrpc/p2p/detail/PeerNetworkSender.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
#include <algorithm>
#include <functional>

namespace nsrpc
{

namespace detail
{

const PeerTime initialEarliestTimeout = std::numeric_limits<PeerTime>::max();

// = Peer

Peer::Peer(PeerId peerId, const Addresses& addresses,
    PeerNetworkSender& networkSender,
    PeerMessageHandler& messageHandler, const P2pConfig& p2pConfig) :
    peerId_(peerId),
    addresses_(addresses),
    networkSender_(networkSender),
    messageHandler_(messageHandler),
    p2pConfig_(p2pConfig),
    currentState_(psDisconnected),
    outgoingReliableSequenceNumber_(invalidSequenceNumber),
    outgoingUnreliableSequenceNumber_(invalidSequenceNumber),
    incomingReliableSequenceNumber_(invalidSequenceNumber),
    incomingUnreliableSequenceNumber_(invalidSequenceNumber),
    nextTimeoutCheckTime_(0),
    lastReceiveTime_(0),
    earliestTimeout_(initialEarliestTimeout)
{
    assert(! addresses.empty());

    std::unique(addresses_.begin(), addresses_.end());
    targetAddress_ = addresses_.front();
}


Peer::~Peer()
{
}


void Peer::setResolvedAddress(const ACE_INET_Addr& address)
{
    if (! isPublicAddress(address)) {
        return;
    }

    const Addresses::iterator pos =
        std::find(addresses_.begin(), addresses_.end(), address);
    if (pos != addresses_.end()) {
        addresses_.erase(pos);
    }

    addresses_.push_front(address);
}


void Peer::putOutgoingMessage(srpc::RpcPacketType packetType,
    ACE_Message_Block* mblock, const ACE_INET_Addr& toAddress)
{
    if (isDisconnecting() || isAcknowledgingDisconnect()) {
        return;
    }

    const ACE_INET_Addr targetAddress =
        toAddress.is_any() ? targetAddress_ : toAddress;

    const PeerTime sentTime = getPeerTime();

    if (srpc::isReliable(packetType)) {
        ++outgoingReliableSequenceNumber_;
        ACE_Message_Block* mblockCloned = mblock->clone();
        if (! putOutgoingReliableMessage(
            ReliableMessage(outgoingReliableSequenceNumber_, mblockCloned,
                targetAddress, sentTime))) {
            mblockCloned->release();
            assert(false && "절대로 실패하면 안됨");
        }
    }
    else if (srpc::isUnreliable(packetType)) {
        ++outgoingUnreliableSequenceNumber_;
        putOutgoingUnreliableMessage(
            Message(outgoingUnreliableSequenceNumber_, mblock->clone(),
                targetAddress, sentTime));
    }
    else {
        assert(false);
    }
}


void Peer::putIncomingMessage(const P2pPacketHeader& header,
    ACE_Message_Block* mblock, const ACE_INET_Addr& peerAddress)
{
    if (srpc::isReliable(header.packetType_)) {
        const ReliableMessage message(header.sequenceNumber_, mblock,
            peerAddress, header.sentTime_);
        if (putIncomingReliableMessage(message)) {
            return;
        }
    } else {
        assert(srpc::isUnreliable(header.packetType_));
        const Message message(header.sequenceNumber_, mblock, peerAddress,
            header.sentTime_);
        if (putIncomingUnreliableMessage(message)) {
            return;
        }
    }

    mblock->release();
}


bool Peer::putIncomingReliableMessage(const ReliableMessage& message)
{
    ++stats_.packetsReceivedReliable_;

    if (message.sequenceNumber_ <= incomingReliableSequenceNumber_) {
        NSRPC_LOG_DEBUG4(
            "Peer(P%u): Incoming Old Reliable Message(#%u <= #%u)",
            peerId_, message.sequenceNumber_ ,
            incomingReliableSequenceNumber_);
        messageHandler_.sendAcknowledgement(peerId_, message);
        return false;
    }

    if (incomingReliableMessages_.isExists(message.sequenceNumber_)) {
        NSRPC_LOG_DEBUG3(
            "Peer(P%u): Incoming existent Reliable Message(#%u)",
            peerId_, message.sequenceNumber_);
        return false;
    }

    //NSRPC_LOG_DEBUG3("P2P|IncomingReliable(P%u,#%u)",
    //    peerId_, message.sequenceNumber_);

    incomingReliableMessages_.insert(message);
    return true;
}


bool Peer::putIncomingUnreliableMessage(const Message& message)
{
    ++stats_.packetsReceivedUnreliable_;

    // TODO: 늦게 도착한 Unreliable 패킷을 어떻게 처리할까?
    //if (message.sequenceNumber_ <= incomingUnreliableSequenceNumber_) {
    //    return false;
    //}

    if (incomingUnreliableMessages_.isExists(message.sequenceNumber_)) {
        NSRPC_LOG_DEBUG3(
            "Peer(P%u): Incoming existent Unreliable Message(#%u)",
            peerId_, message.sequenceNumber_);
        return false;
    }

    //NSRPC_LOG_DEBUG3("P2P|IncomingUnreliable(P%u,#%u)",
    //    peerId_, message.sequenceNumber_);

    incomingUnreliableMessages_.insert(message);
    return true;
}


bool Peer::putOutgoingReliableMessage(const ReliableMessage& message)
{
    if (outgoingReliableMessages_.isExists(message.sequenceNumber_)) {
        //assert(false && "tick 처리가 너무 느린 경우?");
        return false;
    }

    outgoingReliableMessages_.insert(message);
    return true;
}


void Peer::putOutgoingUnreliableMessage(const Message& message)
{
    assert(! outgoingUnreliableMessages_.isExists(message.sequenceNumber_));

    outgoingUnreliableMessages_.insert(message);
}


void Peer::sendOutgoingMessages(PeerId fromPeerId, PeerTime currentTime)
{
    if (isDisconnected()) {
        return;
    }

    if ((! sentReliableMessages_.empty()) && shouldCheckTimeout(currentTime)) {
        checkTimeout(currentTime);
    }

    sendOutgoingUnreliableMessages(fromPeerId);

    if (! outgoingReliableMessages_.empty()) {
        sendOutgoingReliableMessages(fromPeerId, currentTime);
    }
    else if (shouldSendPing(currentTime)) {
        sendPing(fromPeerId, currentTime);
    }
}


bool Peer::handleIncomingMessages()
{
    handleIncomingReliableMessages();
    return handleIncomingUnreliableMessages();
}


void Peer::acknowledged(SequenceNumber sequenceNumber,
    PeerTime receivedSentTime)
{
    adjustRoundTripTime(receivedSentTime);

    removeSentReliableMessage(sequenceNumber);
}


bool Peer::disconnect()
{
    if (isDisconnecting() || isDisconnected()) {
        return false;
    }

    releaseMessages();

    if (isConnected()) {
        disconnecting();
    }

    return true;
}


void Peer::putSentReliableMessage(const ReliableMessage& message)
{
    sentReliableMessages_.insert(message);
}


void Peer::sendOutgoingReliableMessages(PeerId fromPeerId,
    PeerTime currentTime)
{
    ReliableMessages::iterator pos = outgoingReliableMessages_.begin();
    const ReliableMessages::iterator end = outgoingReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;
        if (! networkSender_.sendNow(PeerIdPair(fromPeerId, peerId_),
            getAddressPair(message), *message.mblock_, srpc::ptReliable,
            message.sequenceNumber_, message.sentTime_)) {
            break;
        }

        ++stats_.packetsSentReliable_;
        message.adjustRoundTripTimeout(p2pConfig_, rtt_);

        if (sentReliableMessages_.empty()) {
            setNextTimeout(message.getNextTimeout(currentTime));
        }

        putSentReliableMessage(message);

        outgoingReliableMessages_.erase(pos++);
        assert(end == outgoingReliableMessages_.end());
    }
}


void Peer::sendOutgoingUnreliableMessages(PeerId fromPeerId)
{
    Messages::iterator pos = outgoingUnreliableMessages_.begin();
    const Messages::iterator end = outgoingUnreliableMessages_.end();
    for (; pos != end;) {
        Message& message = *pos;
        if (! networkSender_.sendNow(PeerIdPair(fromPeerId, peerId_),
            getAddressPair(message), *message.mblock_, srpc::ptUnreliable,
            message.sequenceNumber_)) {
            break;
        }

        ++stats_.packetsSentUnreliable_;

        message.release();
        outgoingUnreliableMessages_.erase(pos++);
        assert(end == outgoingUnreliableMessages_.end());
    }
}


void Peer::checkTimeout(PeerTime currentTime)
{
    ReliableMessages::iterator pos = sentReliableMessages_.begin();
    const ReliableMessages::iterator end = sentReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;

        if (! message.shouldCheckTimeout(currentTime)) {
            break; // 메세지가 시간(SeqNo) 순으로 정렬되어 있으므로
        }

        if (checkDisconnect(currentTime, message)) {
            disconnecting();
            break;
        }

        retransmit(message);

        if ((message.sequenceNumber_ ==
                (*sentReliableMessages_.begin()).sequenceNumber_) &&
            (sentReliableMessages_.size() >= 2)) {
            setNextTimeout(message.getNextTimeout());
        }

        sentReliableMessages_.erase(pos++);
        assert(end == sentReliableMessages_.end());
    }
}


bool Peer::checkDisconnect(PeerTime currentTime,
    const ReliableMessage& sentReliableMessage)
{
    if (sentReliableMessage.sentTime_ < earliestTimeout_) {
        earliestTimeout_ = sentReliableMessage.sentTime_;
    }

    const PeerTime timeoutDiff = getElapsedTime(currentTime, earliestTimeout_);

    if (timeoutDiff < p2pConfig_.maxDisconnectTimeout_) {
        if (! sentReliableMessage.isTimeout()) {
            return false;
        }

        if (timeoutDiff < p2pConfig_.minDisconnectTimeout_) {
            return false;
        }
    }

    NSRPC_LOG_DEBUG9("checkDisconnect(P%u, %u, %u, <%u, %u, %u>, [%u,%u])",
        peerId_, currentTime, earliestTimeout_,
        sentReliableMessage.sentTime_,
        sentReliableMessage.roundTripTimeout_,
        sentReliableMessage.roundTripTimeoutLimit_,
        p2pConfig_.minDisconnectTimeout_, p2pConfig_.maxDisconnectTimeout_);

    return true;
}


void Peer::retransmit(ReliableMessage& message)
{
    message.increaseRoundTripTimeout();
    if (! putOutgoingReliableMessage(message)) {
        message.release();
        return;
    }

    ++stats_.packetsLost_;

    NSRPC_LOG_DEBUG8("retransmit(P%u, #%u, %d, %d~%d, %d, %d)",
        peerId_, message.sequenceNumber_, stats_.packetsLost_,
        message.roundTripTimeout_, message.roundTripTimeoutLimit_,
        rtt_.meanRoundTripTime_, rtt_.roundTripTimeVariance_);
}


bool Peer::shouldSendPing(PeerTime currentTime) const
{
    if (isRelayServer(peerId_)) {
        return false;
    }

    if (! isConnected()) {
        return false;
    }

    if (! sentReliableMessages_.empty()) {
        return false;
    }
    
    return (currentTime - lastReceiveTime_) >= p2pConfig_.pingInterval_;
}


void Peer::sendPing(PeerId fromPeerId, PeerTime currentTime)
{
    assert(! isRelayServer(peerId_));

    if (outgoingReliableMessages_.empty()) {
        messageHandler_.sendPing(peerId_);
        sendOutgoingReliableMessages(fromPeerId, currentTime);
    }
}


void Peer::handleIncomingReliableMessages()
{
#ifdef HANDLE_ENTIRELY
    ReliableMessages::iterator pos = incomingReliableMessages_.begin();
    const ReliableMessages::iterator end = incomingReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;
        if (message.sequenceNumber_ == (incomingReliableSequenceNumber_ + 1)) {
            if (messageHandler_.handleIncomingMessage(peerId_,
                srpc::ptReliable, message)) {
                ++incomingReliableSequenceNumber_;
            }

            message.release();
            incomingReliableMessages_.erase(pos++);
            assert(end == incomingReliableMessages_.end());

            if (isAcknowledgingDisconnect()) {
                break;
            }
        }
        else {
            break;
        }
    }

    if (isAcknowledgingDisconnect()) {
        releaseMessages();
    }
#else
    if (! incomingReliableMessages_.empty()) {
        ReliableMessage& message = *(incomingReliableMessages_.begin());
        if (message.sequenceNumber_ ==
            (incomingReliableSequenceNumber_ + 1)) {
            if (messageHandler_.handleIncomingMessage(peerId_,
                srpc::ptReliable, message)) {
                ++incomingReliableSequenceNumber_;
            }
            message.release();
            incomingReliableMessages_.erase(
                incomingReliableMessages_.begin());
        }
    }

    if (isAcknowledgingDisconnect()) {
        releaseMessages();
    }
#endif
}


// TODO: 순서에 어긋나는 비신뢰 패킷을 어떻게 처리할 것인가?
//  - 현재는 무시한다
bool Peer::handleIncomingUnreliableMessages()
{
    // TODO: 일정 시간 동안 패킷이 쌓이도록 대기???
#ifdef HANDLE_ENTIRELY
    const PeerState savedState = currentState_;
    Messages::iterator pos = incomingUnreliableMessages_.begin();
    const Messages::iterator end = incomingUnreliableMessages_.end();
    for (; pos != end;) {
        Message& message = *pos;
        if (message.sequenceNumber_ >=
            (incomingUnreliableSequenceNumber_ + 1)) {
            if (messageHandler_.handleIncomingMessage(peerId_,
                srpc::ptUnreliable, message)) {
                incomingUnreliableSequenceNumber_ = message.sequenceNumber_;
            }
        }

        message.release();
        incomingUnreliableMessages_.erase(pos++);
        assert(end == incomingUnreliableMessages_.end());
    }

    return (currentState_ == savedState) || (! isDisconnected());
#else
    const PeerState savedState = currentState_;
    if (! incomingUnreliableMessages_.empty()) {
        Message& message = *(incomingUnreliableMessages_.begin());
        if (message.sequenceNumber_ >=
            (incomingUnreliableSequenceNumber_ + 1)) {
            if (messageHandler_.handleIncomingMessage(peerId_,
                srpc::ptUnreliable, message)) {
                incomingUnreliableSequenceNumber_ = message.sequenceNumber_;
            }
        }
        message.release();
        incomingUnreliableMessages_.erase(
            incomingUnreliableMessages_.begin());
    }

    return (currentState_ == savedState) || (! isDisconnected());
#endif
}


void Peer::adjustRoundTripTime(PeerTime receivedSentTime)
{
    const PeerTime currentTime = getPeerTime();
    assert(receivedSentTime <= currentTime);
    const PeerTime roundTripTime = (currentTime - receivedSentTime);

    lastReceiveTime_ = currentTime;
    earliestTimeout_ = initialEarliestTimeout;

    // TODO: throattling

    rtt_.update(roundTripTime);

    NSRPC_LOG_DEBUG5("RTT(P%u) = %u/%u, %u",
        peerId_, roundTripTime, rtt_.meanRoundTripTime_,
        rtt_.roundTripTimeVariance_);

    // TODO: throattling
}


void Peer::removeSentReliableMessage(SequenceNumber sequenceNumber)
{
    // GPG5 참고(수신자가 신뢰 패킷을 순서대로 처리하므로)
    ReliableMessages::iterator pos = sentReliableMessages_.begin();
    const ReliableMessages::iterator end = sentReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;
        if (message.sequenceNumber_ > sequenceNumber) {
            break;
        }

        setNextTimeout(message.getNextTimeout());

        message.release();
        sentReliableMessages_.erase(pos++);
        assert(end == sentReliableMessages_.end());

        if (isAcknowledgingConnect()) {
            if (! messageHandler_.acknowledgedConnect(peerId_)) {
                break;
            }
        }
    }
}


void Peer::releaseMessages()
{
    outgoingReliableMessages_.release();
    outgoingUnreliableMessages_.release();
    sentReliableMessages_.release();
    incomingReliableMessages_.release();
    incomingUnreliableMessages_.release();
}


PeerStats Peer::getStats() const
{
    PeerStats currentStats = stats_;
    currentStats.meanRoundTripTime_ = rtt_.meanRoundTripTime_;
    currentStats.roundTripTimeVariance_ = rtt_.roundTripTimeVariance_;
    return currentStats;
}

} // namespace detail

} // namespace nsrpc
