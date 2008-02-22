#ifndef NSRPC_PEERSTATS_H
#define NSRPC_PEERSTATS_H

#include <srpc/Types.h>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct PeerStats
 * a P2P peer's statistics
 */
struct PeerStats
{
    srpc::UInt32 sentReliablePackets_; ///< 신뢰 보장 패킷 전송 횟수
    srpc::UInt32 sentUnreliablePackets_; ///< 비신뢰 보장 패킷 전송 횟수
    srpc::UInt32 receivedReliablePackets_;  ///< 신뢰 보장 패킷 수신 횟수
    srpc::UInt32 receivedUnreliablePackets_; ///< 비신뢰 보장 패킷 수신 횟수
    srpc::UInt32 lostSendPackets_; ///< 송신 신뢰 보장 패킷 손실 횟수

    srpc::UInt32 droppedSendPackets_; ///< dropped packets by simulator
    srpc::UInt32 droppedRecvPackets_; ///< dropped packets by simulator

    srpc::UInt32 meanRoundTripTime_; ///< mean RTT
    srpc::UInt32 highestRoundTripTime_;

    srpc::UInt32 roundTripTimeVariance_;
    srpc::UInt32 highestRoundTripTimeVariance_;

    PeerStats() :
        sentReliablePackets_(0),
        sentUnreliablePackets_(0),
        receivedReliablePackets_(0), 
        receivedUnreliablePackets_(0),
        lostSendPackets_(0),
        droppedSendPackets_(0),
        droppedRecvPackets_(0),
        meanRoundTripTime_(0),
        highestRoundTripTime_(0),
        roundTripTimeVariance_(0),
        highestRoundTripTimeVariance_(0) {}
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERSTATS_H
