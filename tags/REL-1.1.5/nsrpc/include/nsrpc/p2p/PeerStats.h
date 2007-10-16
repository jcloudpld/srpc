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
    srpc::UInt32 packetsSentReliable_; ///< �ŷ� ���� ��Ŷ ���� Ƚ��
    srpc::UInt32 packetsSentUnreliable_; ///< ��ŷ� ���� ��Ŷ ���� Ƚ��
    srpc::UInt32 packetsReceivedReliable_;  ///< �ŷ� ���� ��Ŷ ���� Ƚ��
    srpc::UInt32 packetsReceivedUnreliable_; ///< ��ŷ� ���� ��Ŷ ���� Ƚ��
    srpc::UInt32 packetsLost_; ///< �ŷ� ���� ��Ŷ �ս� Ƚ��

    srpc::UInt32 meanRoundTripTime_; ///< mean RTT
    srpc::UInt32 roundTripTimeVariance_;

    PeerStats() :
        packetsSentReliable_(0),
        packetsSentUnreliable_(0),
        packetsReceivedReliable_(0), 
        packetsReceivedUnreliable_(0),
        packetsLost_(0) {}
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERSTATS_H
