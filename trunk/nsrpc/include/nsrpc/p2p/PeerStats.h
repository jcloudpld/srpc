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
    srpc::UInt32 packetsSentReliable_; ///< ½Å·Ú º¸Àå ÆÐÅ¶ Àü¼Û È½¼ö
    srpc::UInt32 packetsSentUnreliable_; ///< ºñ½Å·Ú º¸Àå ÆÐÅ¶ Àü¼Û È½¼ö
    srpc::UInt32 packetsReceivedReliable_;  ///< ½Å·Ú º¸Àå ÆÐÅ¶ ¼ö½Å È½¼ö
    srpc::UInt32 packetsReceivedUnreliable_; ///< ºñ½Å·Ú º¸Àå ÆÐÅ¶ ¼ö½Å È½¼ö
    srpc::UInt32 packetsLost_; ///< ½Å·Ú º¸Àå ÆÐÅ¶ ¼Õ½Ç È½¼ö

    srpc::UInt32 meanRoundTripTime_; ///< mean RTT
    srpc::UInt32 highestRoundTripTime_;

    srpc::UInt32 roundTripTimeVariance_;
    srpc::UInt32 highestRoundTripTimeVariance_;

    PeerStats() :
        packetsSentReliable_(0),
        packetsSentUnreliable_(0),
        packetsReceivedReliable_(0), 
        packetsReceivedUnreliable_(0),
        packetsLost_(0),
        meanRoundTripTime_(0),
        highestRoundTripTime_(0),
        roundTripTimeVariance_(0),
        highestRoundTripTimeVariance_(0) {}
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERSTATS_H
