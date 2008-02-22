#ifndef NSRPC_PEER_H
#define NSRPC_PEER_H

#include "SystemService.h"
#include "P2pMessages.h"
#include <nsrpc/p2p/PeerId.h>
#include <nsrpc/p2p/PeerStats.h>
#include <nsrpc/p2p/detail/P2pAddress.h>
#include <nsrpc/utility/SmartPtr.h>
#include <nsrpc/utility/Logger.h>

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
 * @class Peer
 * P2P Peer
 */
class Peer : public SharedObject
{
    enum PeerState {
        psDisconnected, ///< 연결이 안된 상태
        psAcknowledgingConnect, ///< 연결 요청에 대한 응답 중
        psConnected, ///< 연결되어 있는 상태
        psDisconnecting, ///< 연결 해제 중
        psAcknowledgingDisconnect, ///< 연결 해제 요청에 대한 응답 중
    };

    typedef MessageSet<Message> Messages;
    typedef MessageSet<ReliableMessage> ReliableMessages;
    typedef MessageSet<DelayedOutboundMessage> DelayedOutboundMessages;
    typedef MessageSet<DelayedInboundMessage> DelayedInboundMessages;
public:

    Peer(PeerId peerId, const Addresses& addresses,
        PeerNetworkSender& networkSender,
        PeerMessageHandler& messageHandler,
        const P2pConfig& p2pConfig);

    void setTargetAddress(const ACE_INET_Addr& address) {
        targetAddress_ = address;
    }

    void setResolvedAddress(const ACE_INET_Addr& address);

    bool disconnect();

    void putOutgoingMessage(srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, const ACE_INET_Addr& toAddress);

    void putIncomingMessage(const P2pPacketHeader& header,
        ACE_Message_Block* mblock, const ACE_INET_Addr& peerAddress);

    void sendOutgoingMessages(PeerId fromPeerId);

    bool handleIncomingMessages();

    void acknowledged(SequenceNumber sequenceNumber,
        PeerTime receivedSentTime);

    PeerId getPeerId() const {
        return peerId_;
    }

    RPeerInfo getPeerInfo() const {
        return RPeerInfo(peerId_, getPeerAddresses());
    }

    RAddresses getPeerAddresses() const {
        return toAddresses(addresses_);
    }

    const ACE_INET_Addr& getTargetAddress() const {
        return targetAddress_;
    }

    PeerStats getStats() const;
public:
    // = 피어 상태 관련
    bool isDisconnected() const {
        return currentState_ == psDisconnected;
    }

    void acknowledgingConnect() {
        assert(isDisconnected());
        currentState_ = psAcknowledgingConnect;
        assert(incomingReliableSequenceNumber_ == 0);
        ++incomingReliableSequenceNumber_;
    }

    void connected(bool messageSent = false) {
        assert(isDisconnected());
        currentState_ = psConnected;
        if (messageSent) {
            assert(outgoingReliableSequenceNumber_ == 0);
            ++outgoingReliableSequenceNumber_;
            ++stats_.sentReliablePackets_;
            assert(incomingReliableSequenceNumber_ == 0);
            ++incomingReliableSequenceNumber_;
        }
    }

    bool isConnected() const {
        return currentState_ != psDisconnected;
    }

    void acknowledgeConnect() {
        assert(isAcknowledgingConnect());
        currentState_ = psConnected;
    }

    bool isDisconnecting() const {
        return currentState_ == psDisconnecting;
    }

    void acknowledgingDisconnect() {
        assert(isConnected());
        currentState_ = psAcknowledgingDisconnect;
    }

    bool isAcknowledgingDisconnect() const {
        return currentState_ == psAcknowledgingDisconnect;
    }
private:
    bool isAcknowledgingConnect() const {
        return currentState_ == psAcknowledgingConnect;
    }

    void disconnecting() {
        currentState_ = psDisconnecting;
    }
protected:
    ~Peer();
private:
    bool putOutgoingReliableMessage(const ReliableMessage& message);
    void putOutgoingUnreliableMessage(const Message& message);
    void putSentReliableMessage(const ReliableMessage& message);

    bool putIncomingMessageDirectly(const P2pPacketHeader& header,
        ACE_Message_Block* mblock, const ACE_INET_Addr& peerAddress);
    bool putIncomingReliableMessage(const ReliableMessage& message);
    bool putIncomingUnreliableMessage(const Message& message);

    void sendOutgoingReliableMessages(PeerId fromPeerId);
    void sendOutgoingUnreliableMessages(PeerId fromPeerId);

    void handleIncomingReliableMessage();
    bool handleIncomingUnreliableMessage();

    void sendOutgoingDelayedMessages(DelayedOutboundMessages& messages);
    void handleIncomingDelayedMessages(DelayedInboundMessages& messages);

    void checkTimeout();
    bool checkDisconnect(const ReliableMessage& sentReliableMessage);
    void retransmit(ReliableMessage& message);

    void sendPing(PeerId fromPeerId);

    bool removeSentReliableMessage(SequenceNumber sequenceNumber);
    void adjustRoundTripTime(PeerTime receivedSentTime);

    void setNextTimeoutCheckTime(PeerTime timeout);

    bool send(PeerId fromPeerId, Message& message,
        srpc::RpcPacketType packetType, bool shouldReleaseMessageBlock);
    bool sendNow(const PeerIdPair& peerIdPair,
        const AddressPair& addressPair, Message& message,
        srpc::RpcPacketType packetType, bool shouldReleaseMessageBlock);

    void releaseMessages();

    bool shouldSendPing() const;

    AddressPair getAddressPair(const Message& message) const {
        return AddressPair(message.peerAddress_, addresses_.front());
    }

    bool shouldCheckTimeout() const {
        return getPeerTime() >= nextTimeoutCheckTime_;
    }

    DelayedOutboundMessages& getDelayedOutboundMessages(
        srpc::RpcPacketType packetType) {
        return (packetType == srpc::ptReliable) ?
            delayedOutgoingReliableMessages_ :
            delayedOutgoingUnreliableMessages_;
    }

    DelayedInboundMessages& getDelayedInboundMessages(
        srpc::RpcPacketType packetType) {
        return (packetType == srpc::ptReliable) ?
            delayedIncomingReliableMessages_ :
            delayedIncomingUnreliableMessages_;
    }
private:
    PeerId peerId_;
    ACE_INET_Addr targetAddress_;
    Addresses addresses_;

    PeerNetworkSender& networkSender_;
    PeerMessageHandler& messageHandler_;
    const P2pConfig& p2pConfig_;

    PeerState currentState_;

    ReliableMessages outgoingReliableMessages_;
    ReliableMessages sentReliableMessages_;
    Messages outgoingUnreliableMessages_;

    ReliableMessages incomingReliableMessages_;
    Messages incomingUnreliableMessages_;

    SequenceNumber outgoingReliableSequenceNumber_;
    SequenceNumber outgoingUnreliableSequenceNumber_;
    SequenceNumber incomingReliableSequenceNumber_;
    SequenceNumber incomingUnreliableSequenceNumber_;

    DelayedOutboundMessages delayedOutgoingReliableMessages_;
    DelayedOutboundMessages delayedOutgoingUnreliableMessages_;

    DelayedInboundMessages delayedIncomingReliableMessages_;
    DelayedInboundMessages delayedIncomingUnreliableMessages_;

    RoundTripTime rtt_; ///< RoundTripTime 관련 정보
    PeerTime nextTimeoutCheckTime_; ///< 다음번 시간 초과 검사 시간
    PeerTime nextTimeoutCheckTimeUpdatedTime_;
    PeerTime lastReceiveTime_; ///< 가장 최근 Ack 메세지 수신 시간
    PeerTime earliestSentTimeout_; ///< 가장 오래된 신뢰성 패킷 전송 시간

    PeerStats stats_;
};

typedef SmartPtr<Peer> PeerPtr;

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEER_H
