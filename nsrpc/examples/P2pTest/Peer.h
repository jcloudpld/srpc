#ifndef P2PTEST_PEER_H
#define P2PTEST_PEER_H

#include "PeerService.h"
#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/p2p/detail/PeerTime.h>

class Config;

namespace nsrpc
{
class P2pSession;
} // namespace nsrpc


/**
 * @class Peer
 * P2P Peer.
 */
class Peer : public RpcPeerService,
    public nsrpc::P2pEventHandler,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    enum {
        tickInterval = 100,
        printInterval = 2000,
        tickProcessingTime = 0
    };

    typedef std::map<nsrpc::PeerId, srpc::UInt32> TickMap;
    typedef std::vector<nsrpc::PeerId> PeerIds;

    DECLARE_SRPC_EVENT_DISPATCHER(Peer);
public:
    Peer(const Config& config);
    virtual ~Peer();

    bool ready();

    bool tick();
private:
    void printAllStats();
    void printStats(nsrpc::PeerId peerId);
public:
    // = RpcPeerService overriding
    DECLARE_SRPC_P2P_METHOD_1(chat, srpc::RShortString, message);
    DECLARE_SRPC_P2P_METHOD_1(tick, srpc::RUInt32, tick);
    DECLARE_SRPC_P2P_METHOD_0(dummy);
private:
    // = P2pEventHandler overriding
    virtual void onPeerConnected(nsrpc::PeerId peerId);
    virtual void onPeerDisconnected(nsrpc::PeerId peerId);
    virtual void onConnectFailed(nsrpc::PeerId peerId);
    virtual void onAddressResolved(const std::string& ipAddress,
        srpc::UInt16 port);
    virtual void onHostMigrated(nsrpc::PeerId peerId);
private:
    const Config& config_;
    boost::scoped_ptr<nsrpc::P2pSession> p2pSession_;
    bool isResolved_;
    nsrpc::PeerId connectFailedPeerId_;
    srpc::UInt32 tickCount_;
    nsrpc::PeerTime printedTime_;
    nsrpc::PeerTime lastSentTime_;
    TickMap tickMap_;
    PeerIds joiners_;
};

#endif // P2PTEST_PEER_H
