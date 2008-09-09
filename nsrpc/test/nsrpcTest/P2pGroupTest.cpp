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
    CPPUNIT_TEST_SUITE_END();
public:
    P2pGroupTest();

    void setUp();
    void tearDown();
private:
    void testCreateGroup();
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
        true, peer_->open(ACE_DEFAULT_SERVER_PORT + 1));

    peer_->connect(getHostAddresses());

    for (int t = 0; t < (2 * 3); ++t) {
        peer_->tick();
        hostSession_->tick();
    }
}


void P2pGroupTest::tearDown()
{
    P2pSessionTestFixture::tearDown();
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

        for (int t = 0; t < (2 * 3); ++t) {
            peer_->tick();
            hostSession_->tick();
        }
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("group count",
        maxGroupCount + 0, static_cast<int>(hostSession_->getGroups().size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last group id",
        giUnknown + 0, int(hostEventHandler_.getLastGroupInfo().groupId_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("last group id",
        maxGroupCount - 1, int(peerEventHandler_.getLastGroupInfo().groupId_));
}
