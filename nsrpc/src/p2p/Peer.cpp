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

const PeerTime initialEarliestSentTimeout =
    std::numeric_limits<PeerTime>::max();

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
    earliestSentTimeout_(initialEarliestSentTimeout)
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
            assert(false && "����� �����ϸ� �ȵ�");
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
        // ������ ���� ��Ŷ�� �ٽ� �������� �ʵ��� ack ����
        messageHandler_.sendAcknowledgement(peerId_, message);
        return false;
    }

    if (incomingReliableMessages_.isExists(message.sequenceNumber_)) {
        NSRPC_LOG_DEBUG5(
            "Peer(P%u): Incoming existent Reliable Message(#%u > #%u, %u)",
            peerId_, message.sequenceNumber_, incomingReliableSequenceNumber_,
            incomingReliableMessages_.size());
        // ���⿡���� ack�� �����ϸ� �ȵȴ�!!
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

    // TODO: �ʰ� ������ Unreliable ��Ŷ�� ��� ó���ұ�?
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
        //assert(false && "tick ó���� �ʹ� ���� ���?");
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

    if (shouldCheckTimeout(currentTime)) {
        checkTimeout(currentTime);
    }

    sendOutgoingUnreliableMessages(fromPeerId);

    if (shouldSendPing(currentTime)) {
        sendPing(fromPeerId, currentTime);
    }
    else {
        sendOutgoingReliableMessages(fromPeerId, currentTime);
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
    bool isFirstMessage = true;

    ReliableMessages::iterator pos = sentReliableMessages_.begin();
    const ReliableMessages::iterator end = sentReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;

        if (! message.shouldCheckTimeout(currentTime)) {
            break; // �޼����� �ð�(SeqNo) ������ ���ĵǾ� �����Ƿ�
        }

        if (isFirstMessage) {
            if (checkDisconnect(currentTime, message)) {
                disconnecting();
                break;
            }
        }

        retransmit(message);

        if (isFirstMessage) {
            setNextTimeout(message.getNextTimeout());
        }

        isFirstMessage = false;

        sentReliableMessages_.erase(pos++);
        assert(end == sentReliableMessages_.end());
    }
}


// TODO: �۽��� �� ������ ������ ���� ��쵵 �����ؾ� �Ѵ�??
bool Peer::checkDisconnect(PeerTime currentTime,
    const ReliableMessage& sentReliableMessage)
{
    if (earliestSentTimeout_ > sentReliableMessage.sentTime_) {
        earliestSentTimeout_ = sentReliableMessage.sentTime_;
    }

    const PeerTime timeoutDiff =
        getElapsedTime(currentTime, earliestSentTimeout_);

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
        currentTime, earliestSentTimeout_,
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

    NSRPC_LOG_DEBUG9("retransmit(P%u, #%u, %u, %u~%u, %u/%u, %u)",
        peerId_, message.sequenceNumber_, message.sentTime_,
        message.roundTripTimeout_, message.roundTripTimeoutLimit_,
        rtt_.meanRoundTripTime_, rtt_.roundTripTimeVariance_,
        sentReliableMessages_.size());
}


bool Peer::shouldSendPing(PeerTime currentTime) const
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
    
    return getElapsedTime(currentTime, lastReceiveTime_) >=
        p2pConfig_.pingInterval_;
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
#endif
}


// TODO: ������ ��߳��� ��ŷ� ��Ŷ�� ��� ó���� ���ΰ�?
//  - ����� �����Ѵ�
bool Peer::handleIncomingUnreliableMessages()
{
    // TODO: ���� �ð� ���� ��Ŷ�� ���̵��� ���???
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
#endif
}


bool Peer::removeSentReliableMessage(SequenceNumber sequenceNumber)
{
    bool removed = false;

    ReliableMessages::iterator pos = sentReliableMessages_.begin();
    const ReliableMessages::iterator end = sentReliableMessages_.end();
    for (; pos != end;) {
        ReliableMessage& message = *pos;
        if (message.sequenceNumber_ > sequenceNumber) {
            // GPG5 ����(�����ڰ� �ŷ� ��Ŷ�� ������� ó���ϹǷ�)
            break;
        }

        setNextTimeout(message.getNextTimeout());

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

    return removed;
}


void Peer::adjustRoundTripTime(PeerTime receivedSentTime)
{
    const PeerTime currentTime = getPeerTime();
    assert(receivedSentTime <= currentTime);
    const PeerTime roundTripTime =
        getElapsedTime(currentTime, receivedSentTime);

    lastReceiveTime_ = currentTime;
    earliestSentTimeout_ = initialEarliestSentTimeout;

    // TODO: throattling

    rtt_.update(roundTripTime);

    //NSRPC_LOG_DEBUG5("RTT(P%u) = %u/%u, %u",
    //    peerId_, roundTripTime, rtt_.meanRoundTripTime_,
    //    rtt_.roundTripTimeVariance_);

    // TODO: throattling
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