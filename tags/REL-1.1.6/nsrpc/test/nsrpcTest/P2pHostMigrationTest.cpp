#include "stdafx.h"
#include "P2pSessionTestFixture.h"
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/utility/AceUtil.h>

/**
* @class P2pHostMigrationTest
*
* P2P Host Migration Test
*/
class P2pHostMigrationTest : public P2pSessionTestFixture
{
    CPPUNIT_TEST_SUITE(P2pHostMigrationTest);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testHostMigration);
    CPPUNIT_TEST(testHostMigrationNextPriorityPeer);
    CPPUNIT_TEST(testDisableHostMigration);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testConnect();
    void testHostMigration();
    void testHostMigrationNextPriorityPeer();
    void testDisableHostMigration();
private:
    void tick(int loop) {
        for (int i = 0; i < loop; ++i) {
            peer1_->tick();
            peer2_->tick();
            peer3_->tick();
            hostSession_->tick();
        }
    }
private:
    TestP2pEventHandler peerEventHandler1_;
    TestP2pEventHandler peerEventHandler2_;
    TestP2pEventHandler peerEventHandler3_;
    P2pSession* peer1_;
    P2pSession* peer2_;
    P2pSession* peer3_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(P2pHostMigrationTest);

const PeerId peerId1_ = 2;
const PeerId peerId2_ = 3;
const PeerId peerId3_ = 4;

void P2pHostMigrationTest::setUp()
{
    P2pSessionTestFixture::setUp();

    hostSession_->close();
    openHost("", 0, true);

    peer1_ = P2pSessionFactory::create(peerId1_, peerEventHandler1_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1 open",
        true, peer1_->open(ACE_DEFAULT_SERVER_PORT + peerId1_));

    peer2_ = P2pSessionFactory::create(peerId2_, peerEventHandler2_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2 open",
        true, peer2_->open(ACE_DEFAULT_SERVER_PORT + peerId2_));

    peer3_ = P2pSessionFactory::create(peerId3_, peerEventHandler3_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer3 open",
        true, peer3_->open(ACE_DEFAULT_SERVER_PORT + peerId3_));

    peer1_->connect(getHostAddresses());
    peer2_->connect(getHostAddresses());

    tick(3 * 3);
}


void P2pHostMigrationTest::tearDown()
{
    P2pSessionTestFixture::tearDown();

    delete peer3_;
    delete peer2_;
    delete peer1_;
}


void P2pHostMigrationTest::testConnect()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("host: three peer",
        3, static_cast<int>(hostSession_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1: three peer",
        3, static_cast<int>(peer1_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2: three peer",
        3, static_cast<int>(peer2_->getPeerCount()));
}


void P2pHostMigrationTest::testHostMigration()
{
    hostSession_->close();

    tick(3 * 3);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1: two peer",
        2, static_cast<int>(peer1_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2: two peer",
        2, static_cast<int>(peer2_->getPeerCount()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1: host migrated",
        peerId1_, peerEventHandler1_.getNewHostPeerId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2: host migrated",
        peerId1_, peerEventHandler2_.getNewHostPeerId());
}


void P2pHostMigrationTest::testHostMigrationNextPriorityPeer()
{
    peer1_->close();
    hostSession_->close();

    tick(3 * 4);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2: two peer",
        1, static_cast<int>(peer2_->getPeerCount()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2: host migrated",
        peerId2_, peerEventHandler2_.getNewHostPeerId());
}


void P2pHostMigrationTest::testDisableHostMigration()
{
    peer1_->close();
    peer2_->close();

    hostSession_->close();
    openHost("", 0, false);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1 open",
        true, peer1_->open(ACE_DEFAULT_SERVER_PORT + peerId1_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2 open",
        true, peer2_->open(ACE_DEFAULT_SERVER_PORT + peerId2_));
    peer1_->connect(getHostAddresses());
    peer2_->connect(getHostAddresses());

    tick(3 * 3);

    hostSession_->close();

    tick(3 * 3);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1: two peer",
        2, static_cast<int>(peer1_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2: two peer",
        2, static_cast<int>(peer2_->getPeerCount()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1: host not migrated",
        invalidPeerId, peerEventHandler1_.getNewHostPeerId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2: host not migrated",
        invalidPeerId, peerEventHandler2_.getNewHostPeerId());
}
