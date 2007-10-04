#ifndef NSRPC_P2PMESSAGES_H
#define NSRPC_P2PMESSAGES_H

#include <nsrpc/p2p/detail/PeerTime.h>
#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <nsrpc/p2p/P2pConfig.h>
#include <hash_set>
#include <deque>

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
    PeerTime highestRoundTripTimeVariance_;
    PeerTime meanRoundTripTime_; ///< RTT 평균값
    PeerTime roundTripTimeVariance_; ///< RTT 변화량

    RoundTripTime() :
        lowestRoundTripTime_(0),
        highestRoundTripTimeVariance_(0),
        meanRoundTripTime_(0),
        roundTripTimeVariance_(0) {}

    void update(PeerTime roundTripTime) {
        roundTripTimeVariance_ -= (roundTripTimeVariance_ / 4);
        PeerTime rttDiff = 0;
        if (roundTripTime >= meanRoundTripTime_) {
            rttDiff = roundTripTime - meanRoundTripTime_;
            meanRoundTripTime_ += (rttDiff / 8);
        }
        else {
            rttDiff = meanRoundTripTime_ - roundTripTime;
            meanRoundTripTime_ -= (rttDiff / 8);
        }
        roundTripTimeVariance_ += (rttDiff / 4);

        if (meanRoundTripTime_ < lowestRoundTripTime_) {
            lowestRoundTripTime_ = meanRoundTripTime_;
        }

        if (roundTripTimeVariance_ > highestRoundTripTimeVariance_) {
            highestRoundTripTimeVariance_ = roundTripTimeVariance_;
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

    Message() :
        sequenceNumber_(invalidSequenceNumber),
        mblock_(0),
        sentTime_(0) {}

    explicit Message(SequenceNumber sequenceNumber,
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

    operator size_t() const {
        return sequenceNumber_;
    }
};


/**
 * @struct ReliableMessage
 */
struct ReliableMessage : Message
{
    PeerTime roundTripTimeout_;
    PeerTime roundTripTimeoutLimit_;

    ReliableMessage() :
        roundTripTimeout_(0),
        roundTripTimeoutLimit_(0) {}

    explicit ReliableMessage(
        SequenceNumber sequenceNumber,
        ACE_Message_Block* mblock = 0,
        const ACE_INET_Addr& peerAddress = ACE_INET_Addr(),
        PeerTime sentTime = 0) :
        Message(sequenceNumber, mblock, peerAddress, sentTime),
        roundTripTimeout_(0),
        roundTripTimeoutLimit_(0) {}

    void adjustRoundTripTimeout(const P2pConfig& p2pConfig,
        const RoundTripTime& rtt) {
        if (roundTripTimeout_ == 0) {
            PeerTime roundTripTime = rtt.meanRoundTripTime_;
            if (roundTripTime == 0) {
                roundTripTime = p2pConfig.defaultRtt_;
            }
            roundTripTimeout_ = roundTripTime +
                (rtt.roundTripTimeVariance_ * p2pConfig.roundTipTimeoutFactor_);
            roundTripTimeoutLimit_ =
                p2pConfig.roundTripTimeoutLimitFactor_ * roundTripTimeout_;
        }
    }

    void increaseRoundTripTimeout() {
        roundTripTimeout_ += roundTripTimeout_; //+= (roundTripTimeout_ / 2);
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

    UnknownReliableMessage() :
        peerId_(invalidPeerId) {}

    explicit UnknownReliableMessage(
        SequenceNumber sequenceNumber,
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

    UnknownUnreliableMessage() :
        peerId_(invalidPeerId) {}

    explicit UnknownUnreliableMessage(
        SequenceNumber sequenceNumber,
        ACE_Message_Block* mblock = 0,
        const ACE_INET_Addr& peerAddress = ACE_INET_Addr(),
        PeerTime sentTime = 0,
        PeerId peerId = invalidPeerId) :
        Message(sequenceNumber, mblock, peerAddress, sentTime),
        peerId_(peerId) {}
};


/**
 * @class ReliableMessages
 */
template <class MessageType>
class MessageSet : public stdext::hash_set<MessageType>
{
public:
    ~MessageSet() {
        release();
    }

    bool isExists(SequenceNumber sequenceNumber) const {
        return getMessage(sequenceNumber) != end();
    }

    iterator getMessage(SequenceNumber sequenceNumber) {
        return find(MessageType(sequenceNumber));
    }

    const_iterator getMessage(SequenceNumber sequenceNumber) const {
        return find(MessageType(sequenceNumber));
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
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PMESSAGES_H
