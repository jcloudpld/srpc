#ifndef P2PTEST_CONFIG_H
#define P2PTEST_CONFIG_H

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
        packetLossRate_(0.0f) {}

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

    float getPacketLossRate() const {
        return packetLossRate_;
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
    float packetLossRate_;
};

#endif // P2PTEST_CONFIG_H
