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
        psDisconnected, ///< ������ �ȵ� ����
        psAcknowledgingConnect, ///< ���� ��û�� ���� ���� ��� ��
        psConnected, ///< ����Ǿ� �ִ� ����
        psDisconnecting, ///< ���� ���� ��
        psAcknowledgingDisconnect, ///< ���� ���� ��û�� ���� ���� ��� ��
    };

    typedef MessageSet<Message> Messages;
    typedef MessageSet<ReliableMessage> ReliableMessages;
    typedef MessageSet<DelayedOutboundMessage> DelayedOutboundMessages;
    typedef MessageSet<DelayedInboundMessage> DelayedInboundMessages;
public:

    Peer(PeerId peerId, const Addresses& addresses, P2pOptions p2pOptions,
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

    bool sendOutgoingMessages(PeerId fromPeerId);

    bool handleIncomingMessages();

    void acknowledged(SequenceNumber sequenceNumber,
        PeerTime receivedSentTime);

    void addP2pOptions(P2pOptions p2pOptions) {
        p2pOptions_ = static_cast<P2pOptions>(p2pOptions_ | p2pOptions);
    }

    PeerId getPeerId() const {
        return peerId_;
    }

    P2pOptions getP2pOptions() const {
        return p2pOptions_;
    }

    bool isAllowed(P2pOptions p2pOptions) const {
        if (p2pOptions == poNone) {
            return true;
        }

        return (p2pOptions & p2pOptions_) != 0;
    }

    RPeerInfo getPeerInfo() const {
        return RPeerInfo(peerId_, getPeerAddresses(), p2pOptions_);
    }

    RAddresses getPeerAddresses() const {
        return toAddresses(addresses_);
    }

    const ACE_INET_Addr& getTargetAddress() const {
        return targetAddress_;
    }

    PeerStats getStats() const;
    std::string getStatsString() const;
public:
    // = �Ǿ� ���� ����

    void acknowledgingConnect();

    void connected(bool messageSent = false);

    void acknowledgeConnect() {
        assert(isAcknowledgingConnect());
        currentState_ = psConnected;
    }

    void acknowledgingDisconnect() {
        assert(isConnected());
        currentState_ = psAcknowledgingDisconnect;
    }

    bool isDisconnected() const {
        return currentState_ == psDisconnected;
    }

    bool isConnected() const {
        return currentState_ != psDisconnected;
    }

    bool isDisconnecting() const {
        return currentState_ == psDisconnecting;
    }

    bool isAcknowledgingDisconnect() const {
        return currentState_ == psAcknowledgingDisconnect;
    }

    PeerTime getLastReceiveTime() const {
        return lastReceiveTime_;
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

    bool sendOutgoingReliableMessages(PeerId fromPeerId);
    bool sendOutgoingUnreliableMessages(PeerId fromPeerId);

    void handleIncomingReliableMessage();
    bool handleIncomingUnreliableMessage();

    bool sendOutgoingDelayedMessages(DelayedOutboundMessages& messages);
    void handleIncomingDelayedMessages(DelayedInboundMessages& messages);

    void checkTimeout();
    bool checkDisconnect(const ReliableMessage& sentReliableMessage);
    void retransmit(ReliableMessage& message);

    void sendPing();

    void sendAcknowledgement(const Message& message);
    void sendPendingAcknowledgement();

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
    P2pOptions p2pOptions_;

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

    AcknowledgementMessage lastAcknowledgementMessage_;

    RoundTripTime rtt_; ///< RoundTripTime ���� ����
    PeerTime nextTimeoutCheckTime_; ///< ������ �ð� �ʰ� �˻� �ð�
    PeerTime nextTimeoutCheckTimeUpdatedTime_;
    PeerTime lastReceiveAckTime_; ///< ���� �ֱ� Ack �޼��� ���� �ð�
    PeerTime earliestSentTimeout_; ///< ���� ������ �ŷڼ� ��Ŷ ���� �ð�
    PeerTime connectedTime_; ///< ������ �ð�
    PeerTime lastReceiveTime_; ///< ���� �ֱ� �޼��� ���� �ð�

    PeerStats stats_;
};

typedef SmartPtr<Peer> PeerPtr;

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEER_H
