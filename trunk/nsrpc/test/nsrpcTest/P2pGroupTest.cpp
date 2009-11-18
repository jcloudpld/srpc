#include "stdafx.h"
#include "P2pSessionTestFixture.h"
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/p2p/PeerHint.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/SystemUtil.h>

/**
* @class P2pGroupTest
*
* P2P group Test
*/
class P2pGroupTest : public P2pSessionTestFixture
{
public:
    P2pGroupTest() :
        p2pConfig_(nsrpc::P2pConfig::peerDefaultRtt, 80) {}

private:
    virtual void SetUp() {
        P2pSessionTestFixture::SetUp();

        peerSession_.reset(P2pSessionFactory::create(2, peerEventHandler_));
        peerRpcPlugIn_ = new TestRpcPlugIn;
        PlugInPtr plugIn(peerRpcPlugIn_);
        peerSession_->attach(plugIn);

        EXPECT_TRUE(peerSession_->open(ACE_DEFAULT_SERVER_PORT + 2)) <<
            "peer open";
        peerSession_->connect(getHostAddresses());

        doTick();
    }

    virtual void TearDown() {
        peerSession_.reset();

        P2pSessionTestFixture::TearDown();
    }

protected:
    void doTick();

    size_t getConnectDelay() const {
        return p2pConfig_.connectTimeout_ + (p2pConfig_.connectTimeout_ / 2);
    }

protected:
    const P2pConfig p2pConfig_;

    TestP2pEventHandler peerEventHandler_;
    boost::scoped_ptr<P2pSession> peerSession_;
    TestRpcPlugIn* peerRpcPlugIn_;
};


void P2pGroupTest::doTick()
{
    for (int t = 0; t < (2 * 3); ++t) {
        peerSession_->tick();
        hostSession_->tick();
    }
}


TEST_F(P2pGroupTest, testCreateGroup)
{
    EXPECT_FALSE(isValid(peerSession_->createGroup("peer"))) <<
        "only host can create a group";

    EXPECT_EQ(0, int(hostSession_->getGroups().size()));

    for (int i = giFirst; i < maxGroupCount; ++i) {
        std::ostringstream oss;
        oss << i << "_group";
        const GroupId groupId = hostSession_->createGroup(oss.str());
        EXPECT_TRUE(isValid(groupId)) << "created?";
        EXPECT_EQ(i + 1, int(hostSession_->getGroups().size()));

        doTick();
    }

    EXPECT_EQ(maxGroupCount + 0, int(hostSession_->getGroups().size()));

    EXPECT_EQ(giUnknown + 0, hostEventHandler_.getLastGroupInfo().groupId_) <<
        "last group id";
    EXPECT_EQ(maxGroupCount - 1,
        peerEventHandler_.getLastGroupInfo().groupId_) << "last group id";
}


TEST_F(P2pGroupTest, testJoinAGroup)
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    EXPECT_TRUE(isValid(firstGroupId));
    doTick();

    EXPECT_TRUE(hostSession_->joinGroup(firstGroupId)) << "host join";
    doTick();

    const RGroupInfo& hostGroupInfo =
        (*hostSession_->getGroups().begin()).second;
    EXPECT_EQ(1, int(hostGroupInfo.peerIds_.size())) << "peer count";
    EXPECT_EQ(hostSession_->getPeerId(),
        hostGroupInfo.peerIds_.front()) << "joined peer id";

    EXPECT_TRUE(peerSession_->joinGroup(firstGroupId)) << "peer join";
    doTick();

    const RGroupInfo& peerGroupInfo = (*peerSession_->getGroups().begin()).second;
    EXPECT_EQ(2, int(peerGroupInfo.peerIds_.size()));
    EXPECT_EQ(hostSession_->getPeerId(), peerGroupInfo.peerIds_.front());

    EXPECT_EQ(giFirst, hostEventHandler_.getLastJoinedGroupId());
    EXPECT_EQ(peerSession_->getPeerId(),
        hostEventHandler_.getLastJoinedPeerId());

    EXPECT_EQ(giFirst, peerEventHandler_.getLastJoinedGroupId());
    EXPECT_EQ(hostSession_->getPeerId(),
        peerEventHandler_.getLastJoinedPeerId());
}


TEST_F(P2pGroupTest, testJoinEachGroup)
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    EXPECT_TRUE(isValid(firstGroupId));

    const GroupId secondGroupId = hostSession_->createGroup("second");
    EXPECT_TRUE(isValid(secondGroupId));

    EXPECT_EQ(2, int(hostSession_->getGroups().size()));

    doTick();

    EXPECT_TRUE(hostSession_->joinGroup(firstGroupId));
    doTick();

    EXPECT_TRUE(peerSession_->joinGroup(secondGroupId));
    doTick();

    const RGroupInfo& hostGroupInfo =
        (*hostSession_->getGroups().begin()).second;
    EXPECT_EQ(1, int(hostGroupInfo.peerIds_.size()));
    EXPECT_EQ(hostSession_->getPeerId(), hostGroupInfo.peerIds_.front());

    const RGroupInfo& peerGroupInfo = (*peerSession_->getGroups().begin()).second;
    EXPECT_EQ(1, int(peerGroupInfo.peerIds_.size()));
    EXPECT_EQ(hostSession_->getPeerId(), peerGroupInfo.peerIds_.front());

    EXPECT_EQ(giFirst + 1, hostEventHandler_.getLastJoinedGroupId());
    EXPECT_EQ(peerSession_->getPeerId(), hostEventHandler_.getLastJoinedPeerId());

    EXPECT_EQ(giFirst, peerEventHandler_.getLastJoinedGroupId());
    EXPECT_EQ(hostSession_->getPeerId(), peerEventHandler_.getLastJoinedPeerId());
}


TEST_F(P2pGroupTest, testLeaveFromGroup)
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    EXPECT_TRUE(isValid(firstGroupId));
    doTick();

    EXPECT_TRUE(hostSession_->joinGroup(firstGroupId));
    EXPECT_TRUE(peerSession_->joinGroup(firstGroupId));
    doTick();

    EXPECT_TRUE(hostSession_->leaveGroup(firstGroupId));
    doTick();

    EXPECT_EQ(giUnknown, hostEventHandler_.getLastLeftGroupId());
    EXPECT_EQ(giFirst, peerEventHandler_.getLastLeftGroupId());
    EXPECT_EQ(hostSession_->getPeerId(), peerEventHandler_.getLastLeftPeerId());

    EXPECT_TRUE(peerSession_->leaveGroup(firstGroupId));
    doTick();

    EXPECT_EQ(giFirst, hostEventHandler_.getLastLeftGroupId());
    EXPECT_EQ(peerSession_->getPeerId(), hostEventHandler_.getLastLeftPeerId());
}


TEST_F(P2pGroupTest, testDestroyGroup)
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    EXPECT_TRUE(isValid(firstGroupId));

    const GroupId secondGroupId = hostSession_->createGroup("second");
    EXPECT_TRUE(isValid(secondGroupId));

    doTick();

    EXPECT_EQ(2, int(peerSession_->getGroups().size()));

    EXPECT_FALSE(peerSession_->destroyGroup(firstGroupId)) <<
        "only host can destroy a P2P group";

    EXPECT_TRUE(hostSession_->destroyGroup(firstGroupId));
    EXPECT_TRUE(hostSession_->destroyGroup(secondGroupId));
    doTick();

    EXPECT_EQ(0, int(hostSession_->getGroups().size()));
    EXPECT_EQ(0, int(peerSession_->getGroups().size()));

    EXPECT_EQ(giUnknown, hostEventHandler_.getLastDestroyedGroupId());
    EXPECT_EQ(giFirst + 1, peerEventHandler_.getLastDestroyedGroupId());

    EXPECT_FALSE(hostSession_->destroyGroup(firstGroupId));
}


TEST_F(P2pGroupTest, testMulticast)
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    const GroupId secondGroupId = hostSession_->createGroup("second");
    EXPECT_EQ(2, int(hostSession_->getGroups().size()));

    doTick();

    EXPECT_TRUE(hostSession_->joinGroup(firstGroupId));
    doTick();

    EXPECT_TRUE(peerSession_->joinGroup(secondGroupId));
    doTick();

    const PeerHint hint(firstGroupId);
    hostRpcPlugIn_->hello("hi", &hint);
    doTick();

    EXPECT_EQ("", peerRpcPlugIn_->getLastWorld());
    EXPECT_EQ(invalidPeerId, peerRpcPlugIn_->getLastPeerId()) << "from host";

    const PeerHint hint2(secondGroupId);
    hostRpcPlugIn_->hello("hi", &hint2);
    doTick();

    EXPECT_EQ("hi", peerRpcPlugIn_->getLastWorld());
    EXPECT_EQ(PeerId(1), peerRpcPlugIn_->getLastPeerId()) << "from host";
}

