#ifndef VOP2PTEST_PEER_H
#define VOP2PTEST_PEER_H

#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/p2p/detail/PeerTime.h>

class Config;
class OpenAlVoP2pPlugIn;

namespace nsrpc
{
class P2pSession;
} // namespace nsrpc


/**
 * @class Peer
 * P2P Peer.
 */
class Peer : public nsrpc::P2pEventHandler
{
    typedef std::map<nsrpc::PeerId, srpc::UInt32> TickMap;
    typedef std::set<nsrpc::PeerId> PeerIdSet;

public:
    Peer(const Config& config);
    virtual ~Peer();

    bool ready();

    bool tick();
private:
    void printAllStats();
    void printStats(nsrpc::PeerId peerId);

    bool isJoiner(nsrpc::PeerId peerId) const {
        return joiners_.find(peerId) != joiners_.end();
    }
private:
    // = P2pEventHandler overriding
    virtual void onPeerConnected(nsrpc::PeerId peerId);
    virtual void onPeerDisconnected(nsrpc::PeerId peerId);
    virtual void onConnectFailed(nsrpc::PeerId peerId);
    virtual void onAddressResolved(const srpc::String& ipAddress,
        srpc::UInt16 port);
    virtual void onHostMigrated(nsrpc::PeerId peerId);
    virtual void onGroupCreated(const nsrpc::RGroupInfo& groupInfo);
    virtual void onGroupDestroyed(nsrpc::GroupId groupId);
    virtual void onGroupJoined(nsrpc::GroupId groupId, nsrpc::PeerId peerId);
    virtual void onGroupLeft(nsrpc::GroupId groupId, nsrpc::PeerId peerId);
private:
    const Config& config_;
    boost::scoped_ptr<nsrpc::P2pSession> p2pSession_;
    bool isResolved_;
    nsrpc::PeerId connectFailedPeerId_;
    srpc::UInt32 tickCount_;
    nsrpc::PeerTime printedTime_;
    TickMap tickMap_;
    PeerIdSet joiners_;

    OpenAlVoP2pPlugIn* voP2pPlugIn_;
};

#endif // VOP2PTEST_PEER_H
