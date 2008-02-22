#ifndef NSRPC_P2PMESSAGES_H
#define NSRPC_P2PMESSAGES_H

#include <nsrpc/p2p/detail/PeerTime.h>
#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <nsrpc/p2p/detail/P2pAddress.h>
#include <nsrpc/p2p/P2pConfig.h>
#include <srpc/ContainerTypes.h>
#include <srpc/RpcPacketType.h>

class ACE_Message_Block;

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct RoundTripTime
 * RTT (모두 msec 단위)
 * - 신뢰 패킷을 보내고 그에 대한 응답 패킷이 오기까지의 평균 RTT
 */
struct RoundTripTime
{
    srpc::Int32 meanRoundTripTime_; ///< RTT 평균값
    srpc::Int32 highestRoundTripTime_;
    srpc::Int32 roundTripTimeVariance_; ///< RTT 변화량
    srpc::Int32 highestRoundTripTimeVariance_;

    RoundTripTime() :
        meanRoundTripTime_(0),
        highestRoundTripTime_(0),
        roundTripTimeVariance_(0),
        highestRoundTripTimeVariance_(0) {}

    void update(PeerTime rtt) {
        const srpc::Int32 rttDiff = getRttDiff(rtt);
        updateMeanRoundTripTime(isRttUp(rtt) ? rttDiff : -rttDiff);
        updateRoundTripTimeVariance(rttDiff);
    }

private:
    void updateMeanRoundTripTime(srpc::Int32 rtt) {
        meanRoundTripTime_ += (rtt / 8);
        if (meanRoundTripTime_ < 0) {
            meanRoundTripTime_ = 0;
        }

        if (meanRoundTripTime_ > highestRoundTripTime_) {
            highestRoundTripTime_ = meanRoundTripTime_;
        }
    }

    void updateRoundTripTimeVariance(srpc::Int32 variance) {
        roundTripTimeVariance_ -= (roundTripTimeVariance_ / 4);
        roundTripTimeVariance_ += (variance / 4);
        if (roundTripTimeVariance_ < 0) {
            roundTripTimeVariance_ = 0;
        }

        if (roundTripTimeVariance_ > highestRoundTripTimeVariance_) {
            highestRoundTripTimeVariance_ = roundTripTimeVariance_;
        }
    }

    srpc::Int32 getRttDiff(srpc::Int32 rtt) const {
        return isRttUp(rtt) ?
            (rtt - meanRoundTripTime_) : (meanRoundTripTime_ - rtt);
    }

    bool isRttUp(srpc::Int32 rtt) const {
        return rtt > meanRoundTripTime_;
    }
};


/**
 * @struct Message
 */
struct Message
{
    SequenceNumber sequenceNumber_;
    ACE_Message_Block* mblock_;
    ACE_INET_Addr peerAddress_;
    PeerTime sentTime_;

    explicit Message(SequenceNumber sequenceNumber = invalidSequenceNumber,
        ACE_Message_Block* mblock = 0,
        const ACE_INET_Addr& peerAddress = ACE_INET_Addr(),
        PeerTime sentTime = 0) :
        sequenceNumber_(sequenceNumber),
        mblock_(mblock),
        peerAddress_(peerAddress),
        sentTime_(sentTime) {}

    void release() {
        if (mblock_ != 0) {
            mblock_->reset();
            mblock_->release();
            mblock_ = 0;
        }
    }

    bool operator < (const Message& rhs) const {
        return sequenceNumber_ < rhs.sequenceNumber_;
    }
};


/**
 * @struct ReliableMessage
 */
struct ReliableMessage : Message
{
    PeerTime roundTripTimeout_;
    PeerTime roundTripTimeoutLimit_;
    srpc::UInt16 sendAttempts_; ///< 전송 시도를 몇번이나 했는가?

    explicit ReliableMessage(
        SequenceNumber sequenceNumber = invalidSequenceNumber,
        ACE_Message_Block* mblock = 0,
        const ACE_INET_Addr& peerAddress = ACE_INET_Addr(),
        PeerTime sentTime = 0) :
        Message(sequenceNumber, mblock, peerAddress, sentTime),
        roundTripTimeout_(0),
        roundTripTimeoutLimit_(0),
        sendAttempts_(0) {}

    void attemptToSending(const P2pConfig& p2pConfig,
        const RoundTripTime& rtt, PeerTime currentTime) {
        ++sendAttempts_;
        adjustRoundTripTimeout(p2pConfig, rtt);
        sentTime_ = currentTime;
    }

    void increaseRoundTripTimeout() {
        roundTripTimeout_ += (roundTripTimeout_ / 2);
    }

    bool shouldCheckTimeout(PeerTime currentTime) const {
        return getElapsedTime(currentTime, sentTime_) >= roundTripTimeout_;
    }

    bool isTimeout() const {
        return roundTripTimeout_ >= roundTripTimeoutLimit_;
    }

    PeerTime getNextTimeout(PeerTime currentTime = 0) const {
        return ((currentTime > 0) ? currentTime : sentTime_) +
            roundTripTimeout_;
    }

private:
    void adjustRoundTripTimeout(const P2pConfig& p2pConfig,
        const RoundTripTime& rtt) {
        if (! isRoundTripTimeoutAdjusted()) {
            const PeerTime roundTripTime = (rtt.meanRoundTripTime_ > 0) ?
                rtt.meanRoundTripTime_ : p2pConfig.defaultRtt_;
            roundTripTimeout_ = roundTripTime +
                (rtt.roundTripTimeVariance_ * p2pConfig.roundTipTimeoutFactor_);
            roundTripTimeoutLimit_ =
                p2pConfig.roundTripTimeoutLimitFactor_ * roundTripTimeout_;
        }
    }

    bool isRoundTripTimeoutAdjusted() const {
        return roundTripTimeout_ != 0;
    } 
};


/**
* @struct UnknownReliableMessage
* 수신자가 아직 정해져 있지 않은 신뢰 메세지
*/
struct UnknownReliableMessage : Message
{
    PeerId peerId_;

    explicit UnknownReliableMessage(
        SequenceNumber sequenceNumber = invalidSequenceNumber,
        ACE_Message_Block* mblock = 0,
        const ACE_INET_Addr& peerAddress = ACE_INET_Addr(),
        PeerTime sentTime = 0, PeerId peerId = invalidPeerId) :
        Message(sequenceNumber, mblock, peerAddress, sentTime),
        peerId_(peerId) {}
};


/**
* @struct UnknownUnreliableMessage
* 수신자가 아직 정해져 있지 않은 비신뢰 메세지
*/
struct UnknownUnreliableMessage : Message
{
    PeerId peerId_;

    explicit UnknownUnreliableMessage(
        SequenceNumber sequenceNumber = invalidSequenceNumber,
        ACE_Message_Block* mblock = 0,
        const ACE_INET_Addr& peerAddress = ACE_INET_Addr(),
        PeerTime sentTime = 0,
        PeerId peerId = invalidPeerId) :
        Message(sequenceNumber, mblock, peerAddress, sentTime),
        peerId_(peerId) {}
};


/**
 * @class MessageSet
 * - squenceNumber 순으로 오름차순 정렬되어야 한다
 */
template <class MessageType>
class MessageSet : public srpc::Set<MessageType>
{
public:
    ~MessageSet() {
        release();
    }

    bool isExists(SequenceNumber sequenceNumber) const {
        return getMessage(sequenceNumber) != end();
    }

    iterator getMessage(SequenceNumber sequenceNumber) {
        messageForFind_.sequenceNumber_ = sequenceNumber;
        return find(messageForFind_);
    }

    const_iterator getMessage(SequenceNumber sequenceNumber) const {
        messageForFind_.sequenceNumber_ = sequenceNumber;
        return find(messageForFind_);
    }

    void release(SequenceNumber sequenceNumber) {
        const iterator pos = getMessage(sequenceNumber);
        if (pos != end()) {
            MessageType& msg = *pos;
            msg.release();
            erase(pos);
        }
    }

    void releaseTo(SequenceNumber sequenceNumber) {
        iterator pos = begin();
        const iterator endPos = end();
        while (pos != endPos) {
            MessageType& msg = *pos;
            if (msg.sequenceNumber_ > sequenceNumber) {
                break;
            }
            msg.release();
            erase(pos++);
        }
    }

    void release() {
        std::for_each(begin(), end(), std::mem_fun_ref(&MessageType::release));
        clear();
    }
private:
    /// 불필요한 임시 변수 생성을 막기 위해 멤버 변수를 이용함
    mutable MessageType messageForFind_;
};


/**
* @struct DelayedOutboundMessage
* 전송이 지연된 메세지
*/
struct DelayedOutboundMessage : Message
{
    PeerIdPair peerIdPair_;
    ACE_INET_Addr targetAddress_;
    srpc::RpcPacketType packetType_;
    PeerTime fireTime_;

    DelayedOutboundMessage() :
        packetType_(srpc::ptUnknown),
        fireTime_(0) {}

    DelayedOutboundMessage(Message& message,
        const AddressPair& addressPair, const PeerIdPair& peerIdPair,
        srpc::RpcPacketType packetType, PeerTime fireTime) :
        Message(message),
        peerIdPair_(peerIdPair),
        targetAddress_(addressPair.targetAddress_),
        packetType_(packetType),
        fireTime_(fireTime) {
        mblock_ = mblock_->clone(); // 주의!!!
    }

    AddressPair getAddressPair() const {
        return AddressPair(targetAddress_, peerAddress_);
    }

    bool shouldFire(PeerTime currentTime) const {
        return fireTime_ <= currentTime;
    }
};


/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PMESSAGES_H
