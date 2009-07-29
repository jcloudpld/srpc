#ifndef NSRPC_P2PSESSIONTESTFIXTURE_H
#define NSRPC_P2PSESSIONTESTFIXTURE_H

#include "AceTestFixture.h"
#include <nsrpc/p2p/P2pSessionFactory.h>
#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/p2p/RpcPlugIn.h>
#include <srpc/RpcP2p.h>
#include <set>

using namespace nsrpc;

/**
 * @class RpcTestService
 */
class RpcTestService
{
public:
    virtual ~RpcTestService() {}

    DECLARE_SRPC_METHOD_1(RpcTestService, hello,
        srpc::RShortString, world);
};


/**
 * @class TestRpcPlugIn
 */
class TestRpcPlugIn : public RpcPlugIn,
    public RpcTestService
{
public:
    TestRpcPlugIn() :
        lastPeerId_(invalidPeerId),
        lastGroupId_(giUnknown) {}

    PeerId getLastPeerId() const {
        return lastPeerId_;
    }

    const std::string& getLastWorld() const {
        return lastWorld_;
    }

public:
    OVERRIDE_SRPC_P2P_METHOD_1(hello, srpc::RShortString, world);

private:
    virtual bool initialize() {
        return true;
    }
    virtual void update() {}

    virtual void onPeerConnected(PeerId /*peerId*/) {}
    virtual void onPeerDisconnected(PeerId /*peerId*/) {}

private:
    PeerId lastPeerId_;
    GroupId lastGroupId_;
    std::string lastWorld_;
};


/**
 * @class TestP2pEventHandler
 */
class TestP2pEventHandler : public P2pEventHandler
{
    typedef std::set<PeerId> PeerIdSet;
public:
    TestP2pEventHandler() :
        connectFailedPeerId_(invalidPeerId),
        addressChanged_(false),
        newHostPeerId_(invalidPeerId),
        lastDestroyedGroupId_(giUnknown),
        lastJoinedGroupId_(giUnknown),
        lastJoinedPeerId_(invalidPeerId),
        lastLeftGroupId_(giUnknown),
        lastLeftPeerId_(invalidPeerId) {}

    bool isConnected(PeerId peerId) const {
        return connectedPeerIds_.find(peerId) != connectedPeerIds_.end();
    }

    int getConnectedPeers() const {
        return static_cast<int>(connectedPeerIds_.size());
    }

    PeerId getConnectFailedPeerId() const {
        return connectFailedPeerId_;
    }

    bool isAddressResolved() const {
        return addressChanged_;
    }

    PeerId getNewHostPeerId() const {
        return newHostPeerId_;
    }

    const RGroupInfo& getLastGroupInfo() const {
        return lastGroupInfo_;
    }

    GroupId getLastDestroyedGroupId() const {
        return lastDestroyedGroupId_;
    }

    GroupId getLastJoinedGroupId() const {
        return lastJoinedGroupId_;
    }

    PeerId getLastJoinedPeerId() const {
        return lastJoinedPeerId_;
    }

    GroupId getLastLeftGroupId() const {
        return lastLeftGroupId_;
    }

    PeerId getLastLeftPeerId() const {
        return lastLeftPeerId_;
    }

private:
    virtual void onPeerConnected(PeerId peerId) {
        connectedPeerIds_.insert(peerId);
    }

    virtual void onPeerDisconnected(PeerId peerId) {
        connectedPeerIds_.erase(peerId);
    }

    virtual void onConnectFailed(PeerId peerId) {
        assert(! isConnected(peerId));
        connectFailedPeerId_ = peerId;
    }

    virtual void onAddressResolved(const srpc::String& /*ipAddress*/,
        srpc::UInt16 /*port*/) {
        addressChanged_ = true;
    }

    virtual void onHostMigrated(PeerId peerId) {
        newHostPeerId_ = peerId;
    }

    virtual void onGroupCreated(const RGroupInfo& groupInfo) {
        lastGroupInfo_ = groupInfo;
    }

    virtual void onGroupDestroyed(GroupId groupId) {
        lastDestroyedGroupId_ = groupId;
    }

    virtual void onGroupJoined(GroupId groupId, PeerId peerId) {
        lastJoinedGroupId_ = groupId;
        lastJoinedPeerId_ = peerId;
    }

    virtual void onGroupLeft(GroupId groupId, PeerId peerId) {
        lastLeftGroupId_ = groupId;
        lastLeftPeerId_ = peerId;
    }
private:
    PeerIdSet connectedPeerIds_;
    PeerId connectFailedPeerId_;
    bool addressChanged_;
    PeerId newHostPeerId_;

    RGroupInfo lastGroupInfo_;
    GroupId lastDestroyedGroupId_;
    GroupId lastJoinedGroupId_;
    PeerId lastJoinedPeerId_;
    GroupId lastLeftGroupId_;
    PeerId lastLeftPeerId_;
};


/**
* @class P2pSessionTestFixture
*
* P2P P2pSession Test
*/
class P2pSessionTestFixture : public AceTestFixture
{
protected:
    virtual void SetUp();
    virtual void TearDown();

protected:
    virtual PeerAddresses getHostAddresses() const;

protected:
    void openHost(const srpc::String& password = "", size_t maxPeers = 0,
        bool hostMigration = false);

protected:
    P2pSession* hostSession_;
    TestP2pEventHandler hostEventHandler_;
    TestRpcPlugIn* hostRpcPlugIn_;
};

#endif // !defined(NSRPC_P2PSESSIONTESTFIXTURE_H)
