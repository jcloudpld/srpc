#ifndef NSRPC_P2PCONFIG_H
#define NSRPC_P2PCONFIG_H

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
        peerRoundTripTimeoutFactor = 2,
        peerRoundTripTimeoutLimitFactor = 10,
        peerMaxDisconnectTimeout = 15000,
        peerMinDisconnectTimeout = 5000,

        /// default Maximum Transmission Unit 
        defaultMtu = 1400,

        peerReconnectTryCount = 3,

        // the minimal interval to resolve the address (milli-seconds)
        minimalAddressResolvingInterval = 500,

        initialAddressResolvingCount = 3,

        // the interval to keep NAT port mapping (milli-seconds)
        defaultNatPortHoldingInterval = (10 * 1000),
    };

    /// default Round trip time value(msec)
    unsigned int defaultRtt_;
    /// connection timeout value(msec)
    unsigned int connectTimeout_;
    /// ping interval value(msec)
    unsigned int pingInterval_;
    /// to calculate RTT timeout(mean RTT * roundTripTimeoutFactor_)
    unsigned int roundTipTimeoutFactor_;
    /// to calculate RTT timeout limit(RTT * roundTripTimeoutLimitFactor_)
    unsigned int roundTripTimeoutLimitFactor_;
    /// max disconnect timeout value(msec)
    unsigned int maxDisconnectTimeout_;
    /// min disconnect timeout value (msec)
    unsigned int minDisconnectTimeout_;

    /// outbound packet drop rate (0.0~1.0, default: 0.0).
    /// Each packet stands the same chance of being dropped,
    /// regardless of other packets.
    /// - high packet loss: 0.1
    float outboundPacketDropRate_;

    /// inbound packet drop rate (0.0~1.0, default: 0.0)
    /// Each packet stands the same chance of being dropped,
    /// regardless of other packets.
    /// - high packet loss: 0.1
    float inboundPacketDropRate_;

    explicit P2pConfig(unsigned int defaultRtt = peerDefaultRtt,
        unsigned int connectTimeout = peerDefaultConnectTimeout,
        unsigned int pingInterval = peerPingInterval,
        unsigned int roundTipTimeoutFactor = peerRoundTripTimeoutFactor,
        unsigned int roundTripTimeoutLimitFactor =
            peerRoundTripTimeoutLimitFactor,
        unsigned int maxDisconnectTimeout = peerMaxDisconnectTimeout,
        unsigned int minDisconnectTimeout = peerMinDisconnectTimeout,
        float outboundPacketDropRate = 0.0f,
        float inboundPacketDropRate = 0.0f) :
        defaultRtt_(defaultRtt),
        connectTimeout_(connectTimeout),
        pingInterval_(pingInterval),
        roundTipTimeoutFactor_(roundTipTimeoutFactor),
        roundTripTimeoutLimitFactor_(roundTripTimeoutLimitFactor),
        maxDisconnectTimeout_(maxDisconnectTimeout),
        minDisconnectTimeout_(minDisconnectTimeout),
        outboundPacketDropRate_(outboundPacketDropRate),
        inboundPacketDropRate_(inboundPacketDropRate){}

    bool shouldDropOutboundPacket() const {
        return (outboundPacketDropRate_ > 0.0f) &&
            (randf() < outboundPacketDropRate_);
    }

    bool shouldDropInboundPacket() const {
        return (inboundPacketDropRate_ > 0.0f) &&
            (randf() < inboundPacketDropRate_);
    }

private:
    // get random number from 0.0 to 1.0
    float randf() const {
        return static_cast<float>(rand()) / RAND_MAX;
    }
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_P2PCONFIG_H
