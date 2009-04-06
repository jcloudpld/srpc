#ifndef VOP2PTEST_CONFIG_H
#define VOP2PTEST_CONFIG_H

#include <nsrpc/p2p/PeerAddress.h>
#include <nsrpc/p2p/PeerId.h>

/**
 * @class Config
 * Configuration.
 */
class Config
{
public:
    Config();

    bool parseArgs(int argc, char* argv[]);

    void printUsage();

    bool shouldRecord() const {
        return shouldRecord_;
    }

    nsrpc::PeerId getPeerId() const {
        return peerId_;
    }

    srpc::UInt16 getListeningPort() const {
        return listeningPort_;
    }

    bool isHost() const {
        return host_;
    }

    const nsrpc::PeerAddress& getRelayServerAddress() const {
        return relayServerAddress_;
    }

    const nsrpc::PeerAddresses& getHostAddresses() const {
        return hostAddresses_;
    }

    srpc::UInt32 getSleepTime() const {
        return sleepTime_;
    }

    float getSendPacketLossRate() const {
        return outboundPacketDropRate_;
    }

    float getRecvPacketLossRate() const {
        return inboundPacketDropRate_;
    }

    srpc::UInt32 getMinOutboundPacketLatency() const {
        return minOutboundPacketLatency_;
    }

    srpc::UInt32 getMaxOutboundPacketLatency() const {
        return maxOutboundPacketLatency_;
    }

    srpc::UInt32 getMinInboundPacketLatency() const {
        return minInboundPacketLatency_;
    }

    srpc::UInt32 getMaxInboundPacketLatency() const {
        return maxInboundPacketLatency_;
    }

    bool isVerbose() const {
        return verbose_;
    }
private:
    bool shouldRecord_;
    nsrpc::PeerId peerId_;
    srpc::UInt16 listeningPort_;
    nsrpc::PeerAddress relayServerAddress_;
    nsrpc::PeerId hostPeerId_;
    nsrpc::PeerAddresses hostAddresses_;
    bool host_;
    bool verbose_;

    srpc::UInt32 sleepTime_;

    float outboundPacketDropRate_;
    float inboundPacketDropRate_;
    srpc::UInt32 minOutboundPacketLatency_;
    srpc::UInt32 maxOutboundPacketLatency_;
    srpc::UInt32 minInboundPacketLatency_;
    srpc::UInt32 maxInboundPacketLatency_;
};

#endif // VOP2PTEST_CONFIG_H
