#include "stdafx.h"
#include "P2pSessionTestFixture.h"
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/SystemUtil.h>

/**
* @class P2pGroupTest
*
* P2P group Test
*/
class P2pGroupTest : public P2pSessionTestFixture
{
    CPPUNIT_TEST_SUITE(P2pGroupTest);
    CPPUNIT_TEST(testCreateGroup);
    CPPUNIT_TEST(testJoinAGroup);
    CPPUNIT_TEST(testJoinEachGroup);
    CPPUNIT_TEST_SUITE_END();
public:
    P2pGroupTest();

    void setUp();
    void tearDown();
private:
    void testCreateGroup();
    void testJoinAGroup();
    void testJoinEachGroup();

private:
    void doTick();

private:
    size_t getConnectDelay() const {
        return p2pConfig_.connectTimeout_ + (p2pConfig_.connectTimeout_ / 2);
    }
private:
    const P2pConfig p2pConfig_;

    TestP2pEventHandler peerEventHandler_;
    boost::scoped_ptr<P2pSession> peer_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(P2pGroupTest);

P2pGroupTest::P2pGroupTest() :
    p2pConfig_(nsrpc::P2pConfig::peerDefaultRtt, 80)
{
}


void P2pGroupTest::setUp()
{
    P2pSessionTestFixture::setUp();

    peer_.reset(P2pSessionFactory::create(2, peerEventHandler_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer open",
        true, peer_->open(ACE_DEFAULT_SERVER_PORT + 2));
    peer_->connect(getHostAddresses());

    doTick();
}


void P2pGroupTest::tearDown()
{
    peer_.reset();

    P2pSessionTestFixture::tearDown();
}


void P2pGroupTest::doTick()
{
    for (int t = 0; t < (2 * 3); ++t) {
        peer_->tick();
        hostSession_->tick();
    }
}


void P2pGroupTest::testCreateGroup()
{
    CPPUNIT_ASSERT_MESSAGE("only host can create a group",
        ! isValid(peer_->createGroup("peer")));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
        0, static_cast<int>(hostSession_->getGroups().size()));

    for (int i = giFirst; i < maxGroupCount; ++i) {
        std::ostringstream oss;
        oss << i << "_group";
        const GroupId groupId = hostSession_->createGroup(oss.str());
        CPPUNIT_ASSERT_MESSAGE("created?",
            isValid(groupId));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
            i + 1, static_cast<int>(hostSession_->getGroups().size()));

        doTick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
        maxGroupCount + 0, static_cast<int>(hostSession_->getGroups().size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last group id",
        giUnknown + 0, int(hostEventHandler_.getLastGroupInfo().groupId_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last group id",
        maxGroupCount - 1, int(peerEventHandler_.getLastGroupInfo().groupId_));
}


void P2pGroupTest::testJoinAGroup()
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    CPPUNIT_ASSERT_MESSAGE("created?",
        isValid(firstGroupId));
    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host join",
        true, hostSession_->joinGroup(firstGroupId));
    doTick();

    const RGroupInfo& hostGroupInfo =
        (*hostSession_->getGroups().begin()).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer count",
        1, int(hostGroupInfo.peerIds_.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("joined peer id",
        hostSession_->getPeerId(), hostGroupInfo.peerIds_.front().get());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer join",
        true, peer_->joinGroup(firstGroupId));
    doTick();

    const RGroupInfo& peerGroupInfo = (*peer_->getGroups().begin()).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer count",
        2, int(peerGroupInfo.peerIds_.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("joined peer id",
        hostSession_->getPeerId(), peerGroupInfo.peerIds_.front().get());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined group id",
        giFirst, hostEventHandler_.getLastJoinedGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined peer id",
        peer_->getPeerId(), hostEventHandler_.getLastJoinedPeerId());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined group id",
        giFirst, peerEventHandler_.getLastJoinedGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined peer id",
        hostSession_->getPeerId(), peerEventHandler_.getLastJoinedPeerId());
}


void P2pGroupTest::testJoinEachGroup()
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    CPPUNIT_ASSERT_MESSAGE("created?",
        isValid(firstGroupId));

    const GroupId secondGroupId = hostSession_->createGroup("second");
    CPPUNIT_ASSERT_MESSAGE("created?",
        isValid(secondGroupId));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
        2, int(hostSession_->getGroups().size()));

    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host join",
        true, hostSession_->joinGroup(firstGroupId));
    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer join",
        true, peer_->joinGroup(secondGroupId));
    doTick();

    const RGroupInfo& hostGroupInfo =
        (*hostSession_->getGroups().begin()).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer count",
        1, int(hostGroupInfo.peerIds_.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("joined peer id",
        hostSession_->getPeerId(), hostGroupInfo.peerIds_.front().get());

    const RGroupInfo& peerGroupInfo = (*peer_->getGroups().begin()).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer count",
        1, int(peerGroupInfo.peerIds_.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("joined peer id",
        hostSession_->getPeerId(), peerGroupInfo.peerIds_.front().get());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined group id",
        giFirst + 1, int(hostEventHandler_.getLastJoinedGroupId()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined peer id",
        peer_->getPeerId(), hostEventHandler_.getLastJoinedPeerId());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined group id",
        giFirst, peerEventHandler_.getLastJoinedGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined peer id",
        hostSession_->getPeerId(), peerEventHandler_.getLastJoinedPeerId());
}
