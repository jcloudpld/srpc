#ifndef NSRPC_P2PMESSAGES_H
#define NSRPC_P2PMESSAGES_H

#include <nsrpc/p2p/detail/PeerTime.h>
#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <nsrpc/p2p/P2pConfig.h>
#include <srpc/ContainerTypes.h>

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
    PeerTime lowestRoundTripTime_;
    PeerTime meanRoundTripTime_; ///< RTT 평균값
    srpc::Int32 roundTripTimeVariance_; ///< RTT 변화량
    srpc::Int32 highestRoundTripTimeVariance_;

    RoundTripTime() :
        lowestRoundTripTime_(0),
        meanRoundTripTime_(0),
        roundTripTimeVariance_(0),
        highestRoundTripTimeVariance_(0) {}

    void update(PeerTime roundTripTime) {
        const srpc::Int32 rttDiff = (roundTripTime - meanRoundTripTime_);
        updateMeanRoundTripTime(rttDiff / 8);
        updateRoundTripTimeVariance(
            (rttDiff / 4) - (roundTripTimeVariance_ / 4));

        if (meanRoundTripTime_ < lowestRoundTripTime_) {
            lowestRoundTripTime_ = meanRoundTripTime_;
        }

        if (roundTripTimeVariance_ > highestRoundTripTimeVariance_) {
            highestRoundTripTimeVariance_ = roundTripTimeVariance_;
        }
    }

private:
    void updateMeanRoundTripTime(srpc::Int32 rtt) {
        meanRoundTripTime_ += rtt;
        if (meanRoundTripTime_ < 0) {
            meanRoundTripTime_ = 0;
        }
    }

    void updateRoundTripTimeVariance(srpc::Int32 variance) {
        roundTripTimeVariance_ += variance;
        if (roundTripTimeVariance_ < 0) {
            roundTripTimeVariance_ = 0;
        }
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

    explicit ReliableMessage(
        SequenceNumber sequenceNumber = invalidSequenceNumber,
        ACE_Message_Block* mblock = 0,
        const ACE_INET_Addr& peerAddress = ACE_INET_Addr(),
        PeerTime sentTime = 0) :
        Message(sequenceNumber, mblock, peerAddress, sentTime),
        roundTripTimeout_(0),
        roundTripTimeoutLimit_(0) {}

    void adjustRoundTripTimeout(const P2pConfig& p2pConfig,
        const RoundTripTime& rtt) {
        if (roundTripTimeout_ == 0) {
            const PeerTime roundTripTime = (rtt.meanRoundTripTime_ > 0) ?
                rtt.meanRoundTripTime_ : p2pConfig.defaultRtt_;
            roundTripTimeout_ = roundTripTime +
                (rtt.roundTripTimeVariance_ * p2pConfig.roundTipTimeoutFactor_);
            roundTripTimeoutLimit_ =
                p2pConfig.roundTripTimeoutLimitFactor_ * roundTripTimeout_;
        }
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

    void release() {
        std::for_each(begin(), end(), std::mem_fun_ref(&MessageType::release));
        clear();
    }
private:
    /// 불필요한 임시 변수 생성을 막기 위해 멤버 변수를 이용함
    mutable MessageType messageForFind_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PMESSAGES_H
