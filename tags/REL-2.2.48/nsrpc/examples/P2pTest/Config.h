#ifndef P2PTEST_CONFIG_H
#define P2PTEST_CONFIG_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/p2p/PeerAddress.h>
#include <nsrpc/p2p/PeerId.h>

/**
 * @class Config
 * Configuration.
 */
class Config
{
public:
    Config() :
        peerId_(nsrpc::invalidPeerId),
        listeningPort_(0),
        host_(false),
        verbose_(false),
        sleepTime_(0),
        outboundPacketDropRate_(0.0f),
        inboundPacketDropRate_(0.0f),
        minOutboundPacketLatency_(0),
        maxOutboundPacketLatency_(0),
        minInboundPacketLatency_(0),
        maxInboundPacketLatency_(0) {}

    bool parseArgs(int argc, char* argv[]);

    void printUsage();

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

#endif // P2PTEST_CONFIG_H
