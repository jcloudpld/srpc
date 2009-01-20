#ifndef NSRPC_P2PCONFIG_H
#define NSRPC_P2PCONFIG_H

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @enum P2pOptions
 * p2p options for each peer
 */
enum P2pOptions
{
    poNone = 0,

    allowVoP2p = 0x01, /// @internal

    poMax = 0xFF
};


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
        peerMaxDisconnectTimeout = 10000,
        peerMinDisconnectTimeout = 5000,

        /// default Maximum Transmission Unit 
        defaultMtu = 1400,

        peerReconnectTryCount = 3,

        // the minimal interval to resolve the address (milli-seconds)
        minimalAddressResolvingInterval = 100,

        // the interval to keep NAT port mapping (milli-seconds)
        defaultNatPortHoldingInterval = (10 * 1000),

        defaultHostAliveConditionTime = 2000
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

    /// outbound packet drop rate (0.0~1.0).
    /// Each packet stands the same chance of being dropped,
    /// regardless of other packets.
    /// - default: 0.0
    /// - ex) high packet loss: 0.1
    float outboundPacketDropRate_;

    /// inbound packet drop rate (0.0~1.0)
    /// Each packet stands the same chance of being dropped,
    /// regardless of other packets.
    /// - default: 0.0
    /// - ex) high packet loss: 0.1
    float inboundPacketDropRate_;

    /// The minimum delay in milliseconds for outbound packets.
    /// The actual delay for an individual packet is chosen randomly between
    /// this minimum value and the maximum latency value.
    /// Use 0 if you do not want to have a lower bound for artificial latency
    /// beyond the real underlying network.
    /// - default: 0
    /// - ex) high min. latency: 100
    unsigned int minOutboundPacketLatency_;

    /// The maximum delay in milliseconds for outbound packets.
    /// The actual delay for an individual packet is chosen randomly between
    /// the minimum latency value and this maximum value.
    /// If this value is lower than the minimum latency value, it is
    /// automatically set to equal the minimum value.
    /// Use 0 if you do not want to have an upper bound for artificial latency
    /// beyond the real underlying network. 
    /// - default: 0
    /// - ex) high max. latency: 400
    unsigned int maxOutboundPacketLatency_;

    /// The minimum delay in milliseconds for inbound packets.
    /// The actual delay for an individual packet is chosen randomly between
    /// this minimum value and the maximum latency value.
    /// Use 0 if you do not want to have a lower bound for artificial latency
    /// beyond the real underlying network.
    /// - default: 0
    /// - ex) high min. latency: 100
    unsigned int minInboundPacketLatency_;

    /// The maximum delay in milliseconds for inbound packets.
    /// The actual delay for an individual packet is chosen randomly between
    /// the minimum latency value and this maximum value.
    /// If this value is lower than the minimum latency value, it is
    /// automatically set to equal the minimum value.
    /// Use 0 if you do not want to have an upper bound for artificial latency
    /// beyond the real underlying network. 
    /// - default: 0
    /// - ex) high max. latency: 400
    unsigned int maxInboundPacketLatency_;

    unsigned int hostAliveConditionTime_;

    /// ctor
    explicit P2pConfig(unsigned int defaultRtt = peerDefaultRtt,
        unsigned int connectTimeout = peerDefaultConnectTimeout,
        unsigned int pingInterval = peerPingInterval,
        unsigned int roundTipTimeoutFactor = peerRoundTripTimeoutFactor,
        unsigned int roundTripTimeoutLimitFactor =
            peerRoundTripTimeoutLimitFactor,
        unsigned int maxDisconnectTimeout = peerMaxDisconnectTimeout,
        unsigned int minDisconnectTimeout = peerMinDisconnectTimeout,
        float outboundPacketDropRate = 0.0f,
        float inboundPacketDropRate = 0.0f,
        unsigned int minOutboundPacketLatency = 0,
        unsigned int maxOutboundPacketLatency = 0,
        unsigned int minInboundPacketLatency = 0,
        unsigned int maxInboundPacketLatency = 0,
        unsigned int hostAliveConditionTime = defaultHostAliveConditionTime) :
        defaultRtt_(defaultRtt),
        connectTimeout_(connectTimeout),
        pingInterval_(pingInterval),
        roundTipTimeoutFactor_(roundTipTimeoutFactor),
        roundTripTimeoutLimitFactor_(roundTripTimeoutLimitFactor),
        maxDisconnectTimeout_(maxDisconnectTimeout),
        minDisconnectTimeout_(minDisconnectTimeout),
        hostAliveConditionTime_(hostAliveConditionTime) {
        setPacketDropRate(outboundPacketDropRate, inboundPacketDropRate);
        setOutboundPacketLatency(minOutboundPacketLatency,
            maxOutboundPacketLatency);
        setInboundPacketLatency(minInboundPacketLatency,
            maxInboundPacketLatency);
    }

    void setPacketDropRate(float outboundPacketDropRate,
        float inboundPacketDropRate) {
        outboundPacketDropRate_ = outboundPacketDropRate;
        if (outboundPacketDropRate_ < 0.0f) {
            outboundPacketDropRate_ = 0.0f;
        }
        if (outboundPacketDropRate_ > 1.0f) {
            outboundPacketDropRate_ = 1.0f;
        }

        inboundPacketDropRate_ = inboundPacketDropRate;
        if (inboundPacketDropRate_ < 0.0f) {
            inboundPacketDropRate_ = 0.0f;
        }
        if (inboundPacketDropRate_ > 1.0f) {
            inboundPacketDropRate_ = 1.0f;
        }
    }

    void setOutboundPacketLatency(unsigned int minOutboundPacketLatency,
        unsigned int maxOutboundPacketLatency) {
        minOutboundPacketLatency_ = minOutboundPacketLatency;
        maxOutboundPacketLatency_ = maxOutboundPacketLatency;

        if (maxOutboundPacketLatency_ < minOutboundPacketLatency_) {
            maxOutboundPacketLatency_ = minOutboundPacketLatency_;
        }
    }

    void setInboundPacketLatency(unsigned int minInboundPacketLatency,
        unsigned int maxInboundPacketLatency) {
        minInboundPacketLatency_ = minInboundPacketLatency;
        maxInboundPacketLatency_ = maxInboundPacketLatency;

        if (maxInboundPacketLatency_ < minInboundPacketLatency_) {
            maxInboundPacketLatency_ = minInboundPacketLatency_;
        }
    }

    bool shouldDropOutboundPacket() const {
        return (outboundPacketDropRate_ > 0.0f) &&
            (getDropRate() < outboundPacketDropRate_);
    }

    bool shouldDropInboundPacket() const {
        return (inboundPacketDropRate_ > 0.0f) &&
            (getDropRate() < inboundPacketDropRate_);
    }

    unsigned int getOutboundPacketLatency() const {
        if (maxOutboundPacketLatency_ <= 0) {
            return 0;
        }

        return getLatency(minOutboundPacketLatency_,
            maxOutboundPacketLatency_);
    }

    unsigned int getInboundPacketLatency() const {
        if (maxInboundPacketLatency_ <= 0) {
            return 0;
        }

        return getLatency(minInboundPacketLatency_,
            maxInboundPacketLatency_);
    }
private:
    // get random number from 0.0 to 1.0
    float getDropRate() const {
        return static_cast<float>(rand()) / RAND_MAX;
    }

    template <typename T>
    T getLatency(T from, T to) const {
        return from +
            static_cast<T>((((to - from) + 1) *
                (static_cast<float>(rand()) / (RAND_MAX + from))));
    }
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_P2PCONFIG_H
