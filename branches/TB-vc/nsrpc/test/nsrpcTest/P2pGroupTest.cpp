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
    CPPUNIT_TEST(testLeaveFromGroup);
    CPPUNIT_TEST(testDestroyGroup);
    CPPUNIT_TEST_SUITE_END();
public:
    P2pGroupTest();

    void setUp();
    void tearDown();
private:
    void testCreateGroup();
    void testJoinAGroup();
    void testJoinEachGroup();
    void testLeaveFromGroup();
    void testDestroyGroup();

private:
    void doTick();

private:
    size_t getConnectDelay() const {
        return p2pConfig_.connectTimeout_ + (p2pConfig_.connectTimeout_ / 2);
    }
private:
    const P2pConfig p2pConfig_;

    TestP2pEventHandler peerEventHandler_;
    boost::scoped_ptr<P2pSession> peerSession_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(P2pGroupTest);

P2pGroupTest::P2pGroupTest() :
    p2pConfig_(nsrpc::P2pConfig::peerDefaultRtt, 80)
{
}


void P2pGroupTest::setUp()
{
    P2pSessionTestFixture::setUp();

    peerSession_.reset(P2pSessionFactory::create(2, peerEventHandler_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer open",
        true, peerSession_->open(ACE_DEFAULT_SERVER_PORT + 2));
    peerSession_->connect(getHostAddresses());

    doTick();
}


void P2pGroupTest::tearDown()
{
    peerSession_.reset();

    P2pSessionTestFixture::tearDown();
}


void P2pGroupTest::doTick()
{
    for (int t = 0; t < (2 * 3); ++t) {
        peerSession_->tick();
        hostSession_->tick();
    }
}


void P2pGroupTest::testCreateGroup()
{
    CPPUNIT_ASSERT_MESSAGE("only host can create a group",
        ! isValid(peerSession_->createGroup("peer")));

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
        true, peerSession_->joinGroup(firstGroupId));
    doTick();

    const RGroupInfo& peerGroupInfo = (*peerSession_->getGroups().begin()).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer count",
        2, int(peerGroupInfo.peerIds_.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("joined peer id",
        hostSession_->getPeerId(), peerGroupInfo.peerIds_.front().get());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined group id",
        giFirst, hostEventHandler_.getLastJoinedGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined peer id",
        peerSession_->getPeerId(), hostEventHandler_.getLastJoinedPeerId());

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
        true, peerSession_->joinGroup(secondGroupId));
    doTick();

    const RGroupInfo& hostGroupInfo =
        (*hostSession_->getGroups().begin()).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer count",
        1, int(hostGroupInfo.peerIds_.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("joined peer id",
        hostSession_->getPeerId(), hostGroupInfo.peerIds_.front().get());

    const RGroupInfo& peerGroupInfo = (*peerSession_->getGroups().begin()).second;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer count",
        1, int(peerGroupInfo.peerIds_.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("joined peer id",
        hostSession_->getPeerId(), peerGroupInfo.peerIds_.front().get());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined group id",
        giFirst + 1, int(hostEventHandler_.getLastJoinedGroupId()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined peer id",
        peerSession_->getPeerId(), hostEventHandler_.getLastJoinedPeerId());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined group id",
        giFirst, peerEventHandler_.getLastJoinedGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last joined peer id",
        hostSession_->getPeerId(), peerEventHandler_.getLastJoinedPeerId());
}


void P2pGroupTest::testLeaveFromGroup()
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    CPPUNIT_ASSERT_MESSAGE("created?",
        isValid(firstGroupId));
    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host join",
        true, hostSession_->joinGroup(firstGroupId));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer join",
        true, peerSession_->joinGroup(firstGroupId));
    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host leave",
        true, hostSession_->leaveGroup(firstGroupId));
    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last left group id",
        giUnknown, hostEventHandler_.getLastLeftGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last left group id",
        giFirst, peerEventHandler_.getLastLeftGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last left peer id",
        hostSession_->getPeerId(), peerEventHandler_.getLastLeftPeerId());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer leave",
        true, peerSession_->leaveGroup(firstGroupId));
    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last left group id",
        giFirst, hostEventHandler_.getLastLeftGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last left peer id",
        peerSession_->getPeerId(), hostEventHandler_.getLastLeftPeerId());
}


void P2pGroupTest::testDestroyGroup()
{
    const GroupId firstGroupId = hostSession_->createGroup("first");
    CPPUNIT_ASSERT_MESSAGE("created?",
        isValid(firstGroupId));

    const GroupId secondGroupId = hostSession_->createGroup("second");
    CPPUNIT_ASSERT_MESSAGE("created?",
        isValid(secondGroupId));

    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
        2, int(peerSession_->getGroups().size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("only host can destroy a P2P group",
        false, peerSession_->destroyGroup(firstGroupId));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("destory first group",
        true, hostSession_->destroyGroup(firstGroupId));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("destory first group",
        true, hostSession_->destroyGroup(secondGroupId));
    doTick();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
        0, int(hostSession_->getGroups().size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
        0, int(peerSession_->getGroups().size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last destroyed group id",
        giUnknown, hostEventHandler_.getLastDestroyedGroupId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last destroyed group id",
        giFirst + 1, int(peerEventHandler_.getLastDestroyedGroupId()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("no group",
        false, hostSession_->destroyGroup(firstGroupId));
}
