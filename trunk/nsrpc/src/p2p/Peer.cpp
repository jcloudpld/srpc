#include "stdafx.h"
#include "Peer.h"
#include "PeerMessageHandler.h"
#include <nsrpc/p2p/detail/PeerNetworkSender.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
#include <algorithm>
#include <functional>

//#define HANDLE_ENTIRELY

namespace nsrpc
{

namespace detail
{

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
    nextTimeoutCheckTimeUpdatedTime_(0),
    lastReceiveTime_(0),
    earliestSentTimeout_(0)
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
        NSRPC_LOG_DEBUG5(
            "Peer(P%u): Incoming Old Reliable Message(#%u <= #%u, %u)",
            peerId_, message.sequenceNumber_ , incomingReliableSequenceNumber_,
            incomingReliableMessages_.size());
        // 상대방이 이전 패킷을 다시 전송하지 않도록 ack 전송
        messageHandler_.sendAcknowledgement(peerId_, message);
        return false;
    }

    if (incomingReliableMessages_.isExists(message.sequenceNumber_)) {
        NSRPC_LOG_DEBUG5(
            "Peer(P%u): Incoming existent Reliable Message(#%u > #%u, %u)",
            peerId_, message.sequenceNumber_, incomingReliableSequenceNumber_,
            incomingReliableMessages_.size());
        // 여기에서는 ack를 전송하면 안된다!!
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


void Peer::sendOutgoingMessages(PeerId fromPeerId)
{
    if (isDisconnected()) {
        return;
    }

    if (shouldCheckTimeout()) {
        checkTimeout();
    }

    sendOutgoingUnreliableMessages(fromPeerId);

    if (shouldSendPing()) {
        sendPing(fromPeerId);
    }
    else {
        sendOutgoingReliableMessages(fromPeerId);
    }
}


bool Peer::handleIncomingMessages()
{
    handleIncomingReliableMessage();
    return handleIncomingUnreliableMessage();
}


void Peer::acknowledged(SequenceNumber sequenceNumber,
    PeerTime receivedSentTime)
{
    if (removeSentReliableMessage(sequenceNumber)) {
        adjustRoundTripTime(receivedSentTime);
    }
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


void Peer::sendOutgoingReliableMessages(PeerId fromPeerId)
{
    const PeerTime currentTime = getPeerTime();

    ReliableMessages::iterator pos = outgoingReliableMessages_.begin();
    const ReliableMessages::iterator end = outgoingReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;

        message.attemptToSending(p2pConfig_, rtt_, currentTime);

        if (sentReliableMessages_.empty()) {
            setNextTimeoutCheckTime(message.getNextTimeout(currentTime));
        }

        putSentReliableMessage(message);

        ++stats_.packetsSentReliable_;

        if (! networkSender_.sendNow(PeerIdPair(fromPeerId, peerId_),
            getAddressPair(message), *message.mblock_, srpc::ptReliable,
            message.sequenceNumber_, message.sentTime_)) {
                break;
        }

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


void Peer::checkTimeout()
{
    const PeerTime currentTime = getPeerTime();

    srpc::UInt16 highestSendAttempts = 0;
    bool isFirstMessage = true;

    ReliableMessages::iterator pos = sentReliableMessages_.begin();
    const ReliableMessages::iterator end = sentReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;

        if (! message.shouldCheckTimeout(currentTime)) {
            break; // 메세지가 시간(SeqNo) 순으로 정렬되어 있으므로
        }

        if (isFirstMessage) {
            if (checkDisconnect(message)) {
                disconnecting();
                break;
            }
            highestSendAttempts = message.sendAttempts_;
        }

        if (message.sendAttempts_ < highestSendAttempts) {
            break;
        }

        retransmit(message);

        if (isFirstMessage && (sentReliableMessages_.size() > 1)) {
            setNextTimeoutCheckTime(message.getNextTimeout());
        }
        isFirstMessage = false;

        sentReliableMessages_.erase(pos++);
        assert(end == sentReliableMessages_.end());
    }
}


bool Peer::checkDisconnect(const ReliableMessage& sentReliableMessage)
{
    if ((earliestSentTimeout_ <= 0) ||
        (earliestSentTimeout_ > sentReliableMessage.sentTime_)) {
        earliestSentTimeout_ = sentReliableMessage.sentTime_;
    }
    assert(earliestSentTimeout_ > 0);
    if (earliestSentTimeout_ <= 0) {
        return false;
    }

    const PeerTime timeoutDiff =
        getElapsedTime(getPeerTime(), earliestSentTimeout_);

    if (timeoutDiff < p2pConfig_.maxDisconnectTimeout_) {
        if (! sentReliableMessage.isTimeout()) {
            return false;
        }

        if (timeoutDiff < p2pConfig_.minDisconnectTimeout_) {
            return false;
        }
    }

    NSRPC_LOG_DEBUG10(
        "checkDisconnect(P%u, #%u, %u, %u, <%u, %u, %u>, [%u,%u])",
        peerId_, sentReliableMessage.sequenceNumber_,
        getPeerTime(), earliestSentTimeout_,
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

    NSRPC_LOG_DEBUG10("retransmit(P%u, #%u, %u, %u, %u~%u, %u/%u, %u)",
        peerId_, message.sequenceNumber_, message.sendAttempts_,
        message.sentTime_,
        message.roundTripTimeout_, message.roundTripTimeoutLimit_,
        rtt_.meanRoundTripTime_, rtt_.roundTripTimeVariance_,
        sentReliableMessages_.size());

    return;
}


bool Peer::shouldSendPing() const
{
    if (isRelayServer(peerId_)) {
        return false;
    }

    if (! isConnected()) {
        return false;
    }

    if (! outgoingReliableMessages_.empty()) {
        return false;
    }

    if (! sentReliableMessages_.empty()) {
        return false;
    }
    
    return getElapsedTime(getPeerTime(), lastReceiveTime_) >=
        p2pConfig_.pingInterval_;
}


void Peer::sendPing(PeerId fromPeerId)
{
    assert(! isRelayServer(peerId_));

    if (outgoingReliableMessages_.empty()) {
        messageHandler_.sendPing(peerId_);
        sendOutgoingReliableMessages(fromPeerId);
    }
}


void Peer::handleIncomingReliableMessage()
{
    if (! incomingReliableMessages_.empty()) {
        const ReliableMessages::iterator firstPos =
            incomingReliableMessages_.begin();
        ReliableMessage& message = *firstPos;
        if (message.sequenceNumber_ ==
            (incomingReliableSequenceNumber_ + 1)) {
            if (messageHandler_.handleIncomingMessage(peerId_,
                srpc::ptReliable, message)) {
                ++incomingReliableSequenceNumber_;
            }

            message.release();
            incomingReliableMessages_.erase(firstPos);
        }
    }

    if (isAcknowledgingDisconnect()) {
        releaseMessages();
    }
}


// TODO: 순서에 어긋나는 비신뢰 패킷을 어떻게 처리할 것인가?
//  - 현재는 무시한다
bool Peer::handleIncomingUnreliableMessage()
{
    // TODO: 일정 시간 동안 패킷이 쌓이도록 대기???

    const PeerState savedState = currentState_;
    if (! incomingUnreliableMessages_.empty()) {
        const Messages::iterator firstPos =
            incomingUnreliableMessages_.begin();
        Message& message = *firstPos;
        if (message.sequenceNumber_ >=
            (incomingUnreliableSequenceNumber_ + 1)) {
            if (messageHandler_.handleIncomingMessage(peerId_,
                srpc::ptUnreliable, message)) {
                incomingUnreliableSequenceNumber_ = message.sequenceNumber_;
            }
        }
        message.release();
        incomingUnreliableMessages_.erase(firstPos);
    }

    return (currentState_ == savedState) || (! isDisconnected());
}


bool Peer::removeSentReliableMessage(SequenceNumber sequenceNumber)
{
    bool removed = false;

    ReliableMessages::iterator pos = sentReliableMessages_.begin();
    const ReliableMessages::iterator end = sentReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;
        if (message.sequenceNumber_ > sequenceNumber) {
            // GPG5 참고(수신자가 신뢰 패킷을 순서대로 처리하므로)
            break;
        }

        outgoingReliableMessages_.releaseTo(message.sequenceNumber_);

        message.release();
        sentReliableMessages_.erase(pos++);
        assert(end == sentReliableMessages_.end());
        removed = true;

        if (isAcknowledgingConnect()) {
            if (! messageHandler_.acknowledgedConnect(peerId_)) {
                break;
            }
        }
    }

    if (removed) {
        if (! sentReliableMessages_.empty()) {
            const ReliableMessage& firstMessage =
                *(sentReliableMessages_.begin());
            setNextTimeoutCheckTime(firstMessage.getNextTimeout());
        }
    }

    return removed;
}


void Peer::adjustRoundTripTime(PeerTime receivedSentTime)
{
    const PeerTime currentTime = getPeerTime();
    assert(receivedSentTime <= currentTime);
    const PeerTime roundTripTime =
        getElapsedTime(currentTime, receivedSentTime);

    lastReceiveTime_ = currentTime;
    earliestSentTimeout_ = 0;

    // TODO: throattling

    rtt_.update(roundTripTime);

    //NSRPC_LOG_DEBUG5("RTT(P%u) = %u/%u, %u",
    //    peerId_, roundTripTime, rtt_.meanRoundTripTime_,
    //    rtt_.roundTripTimeVariance_);

    // TODO: throattling
}


void Peer::setNextTimeoutCheckTime(PeerTime timeout)
{
    const srpc::Int32 diff = timeout - nextTimeoutCheckTime_;
    if (diff < 0) {
        return;
    }

    const PeerTime currentTime = getPeerTime();
    if (nextTimeoutCheckTimeUpdatedTime_ == currentTime) {
        return;
    }
    nextTimeoutCheckTimeUpdatedTime_ = currentTime;

    nextTimeoutCheckTime_ = timeout;
    const srpc::Int32 threshold = rtt_.meanRoundTripTime_;
    if (diff < threshold) {
        nextTimeoutCheckTime_ += threshold;
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
    currentStats.highestRoundTripTime_ = rtt_.highestRoundTripTime_;
    currentStats.roundTripTimeVariance_ = rtt_.roundTripTimeVariance_;
    currentStats.highestRoundTripTimeVariance_ =
        rtt_.highestRoundTripTimeVariance_;
    return currentStats;
}

} // namespace detail

} // namespace nsrpc
