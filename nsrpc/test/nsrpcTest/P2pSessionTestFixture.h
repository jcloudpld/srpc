#ifndef NSRPC_P2PSESSIONTESTFIXTURE_H
#define NSRPC_P2PSESSIONTESTFIXTURE_H

#include "AceTestFixture.h"
#include <nsrpc/p2p/P2pSessionFactory.h>
#include <nsrpc/p2p/P2pEventHandler.h>
#include <set>

using namespace nsrpc;

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
public:
    void setUp() {
        AceTestFixture::setUp();

        hostSession_ = P2pSessionFactory::create(1, hostEventHandler_);
        openHost();
    }

    void tearDown() {
        delete hostSession_;
        AceTestFixture::tearDown();
    }
protected:
    virtual PeerAddresses getHostAddresses() const {
        PeerAddresses addresses;
        addresses.push_back(
            PeerAddress(ACE_LOCALHOST, ACE_DEFAULT_SERVER_PORT));
        return addresses;
    }
protected:
    void openHost(const srpc::String& password = "", size_t maxPeers = 0,
        bool hostMigration = false) {
        CPPUNIT_ASSERT_MESSAGE("host open",
            hostSession_->open(ACE_DEFAULT_SERVER_PORT, password));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("no host",
            false, hostSession_->isHost());
        hostSession_->host(maxPeers, hostMigration);
    }
protected:
    P2pSession* hostSession_;
    TestP2pEventHandler hostEventHandler_;
};

#endif // !defined(NSRPC_P2PSESSIONTESTFIXTURE_H)
