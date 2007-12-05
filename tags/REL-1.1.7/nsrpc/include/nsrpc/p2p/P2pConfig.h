#ifndef NSRPC_P2PCONFIG_H
#define NSRPC_P2PCONFIG_H

#include <srpc/Types.h>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct P2pConfig
 * P2P configuration.
 */
struct P2pConfig
{
    enum P2pDefaultProperties
    {
        peerDefaultRtt = 150,
        peerDefaultConnectTimeout = 2000,
        peerPingInterval = 500,
        peerRoundTripTimeoutFactor = 4,
        peerRoundTripTimeoutLimitFactor = 15,
        peerMaxDisconnectTimeout = 8000,
        peerMinDisconnectTimeout = 5000,

        /// default Maximum Transmission Unit 
        defaultMtu = 1400,

        peerReconnectTryCount = 3,

        // the minimal interval to resolve the address (milli-seconds)
        minimalAddressResolvingInterval = 500,

        initialAddressResolvingCount = 3,

        // the interval to keep NAT port mapping (milli-seconds)
        defaultNatPortHoldingInterval = (10 * 1000)
    };

    /// default Round trip time value(msec)
    const srpc::UInt32 defaultRtt_;
    /// connection timeout value(msec)
    const srpc::UInt32 connectTimeout_;
    /// ping interval value(msec)
    const srpc::UInt32 pingInterval_;
    /// to calculate RTT timeout(mean RTT * roundTripTimeoutFactor_)
    const srpc::UInt32 roundTipTimeoutFactor_;
    /// to calculate RTT timeout limit(RTT * roundTripTimeoutLimitFactor_)
    const srpc::UInt32 roundTripTimeoutLimitFactor_;
    /// max disconnect timeout value(msec)
    const srpc::UInt32 maxDisconnectTimeout_;
    /// min disconnect timeout value (msec)
    const srpc::UInt32 minDisconnectTimeout_;

    explicit P2pConfig(srpc::UInt32 defaultRtt = peerDefaultRtt,
        srpc::UInt32 connectTimeout = peerDefaultConnectTimeout,
        srpc::UInt32 pingInterval = peerPingInterval,
        srpc::UInt32 roundTipTimeoutFactor = peerRoundTripTimeoutFactor,
        srpc::UInt32 roundTripTimeoutLimitFactor =
            peerRoundTripTimeoutLimitFactor,
        srpc::UInt32 maxDisconnectTimeout = peerMaxDisconnectTimeout,
        srpc::UInt32 minDisconnectTimeout = peerMinDisconnectTimeout) :
        defaultRtt_(defaultRtt),
        connectTimeout_(connectTimeout),
        pingInterval_(pingInterval),
        roundTipTimeoutFactor_(roundTipTimeoutFactor),
        roundTripTimeoutLimitFactor_(roundTripTimeoutLimitFactor),
        maxDisconnectTimeout_(maxDisconnectTimeout),
        minDisconnectTimeout_(minDisconnectTimeout) {}
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_P2PCONFIG_H
