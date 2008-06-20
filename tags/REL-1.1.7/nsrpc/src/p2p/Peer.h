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
        psAcknowledgingConnect, ///< ���� ��û�� ���� ���� ��
        psConnected, ///< ����Ǿ� �ִ� ����
        psDisconnecting, ///< ���� ���� ��
        psAcknowledgingDisconnect, ///< ���� ���� ��û�� ���� ���� ��
    };

    typedef MessageSet<Message> Messages;
    typedef MessageSet<ReliableMessage> ReliableMessages;
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

    void sendOutgoingMessages(PeerId fromPeerId, PeerTime currentTime);

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
    // = �Ǿ� ���� ����
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
            ++stats_.packetsSentReliable_;
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

    bool putIncomingReliableMessage(const ReliableMessage& message);
    bool putIncomingUnreliableMessage(const Message& message);

    void sendOutgoingReliableMessages(PeerId fromPeerId,
        PeerTime currentTime);
    void sendOutgoingUnreliableMessages(PeerId fromPeerId);

    void handleIncomingReliableMessages();
    bool handleIncomingUnreliableMessages();

    void checkTimeout(PeerTime currentTime);
    bool checkDisconnect(PeerTime currentTime,
        const ReliableMessage& sentReliableMessage);
    void retransmit(ReliableMessage& message);

    void sendPing(PeerId fromPeerId, PeerTime currentTime);

    void adjustRoundTripTime(PeerTime receivedSentTime);
    void removeSentReliableMessage(SequenceNumber sequenceNumber);

    void setNextTimeout(PeerTime timeout) {
        if (timeout > nextTimeoutCheckTime_) {
            nextTimeoutCheckTime_ = timeout;
        }
    }

    void releaseMessages();

    bool shouldSendPing(PeerTime currentTime) const;

    AddressPair getAddressPair(const Message& message) const {
        return AddressPair(message.peerAddress_, addresses_.front());
    }

    bool shouldCheckTimeout(PeerTime currentTime) const {
        return currentTime >= nextTimeoutCheckTime_;
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

    RoundTripTime rtt_; ///< RoundTripTime ���� ����
    PeerTime nextTimeoutCheckTime_; ///< ������ �ð� �ʰ� �˻� �ð�
    PeerTime lastReceiveTime_; ///< ���� �ֱ� Ack �޼��� ���� �ð�
    PeerTime earliestTimeout_; ///< ���� ������ �ŷڼ� ��Ŷ ���� �ð�

    PeerStats stats_;
};

typedef SmartPtr<Peer> PeerPtr;

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEER_H