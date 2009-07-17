#include "stdafx.h"
#include "P2pSessionTestFixture.h"
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/utility/AceUtil.h>

const PeerId peerId1_ = 2;
const PeerId peerId2_ = 3;
const PeerId peerId3_ = 4;

/**
* @class P2pHostMigrationTest
*
* P2P Host Migration Test
*/
class P2pHostMigrationTest : public P2pSessionTestFixture
{
private:
    virtual void SetUp() {
        P2pSessionTestFixture::SetUp();

        hostSession_->close();
        openHost("", 0, true);

        peer1_ = P2pSessionFactory::create(peerId1_, peerEventHandler1_);
        EXPECT_TRUE(peer1_->open(ACE_DEFAULT_SERVER_PORT + peerId1_));

        peer2_ = P2pSessionFactory::create(peerId2_, peerEventHandler2_);
        EXPECT_TRUE(peer2_->open(ACE_DEFAULT_SERVER_PORT + peerId2_));

        peer3_ = P2pSessionFactory::create(peerId3_, peerEventHandler3_);
        EXPECT_TRUE(peer3_->open(ACE_DEFAULT_SERVER_PORT + peerId3_));

        peer1_->connect(getHostAddresses());
        peer2_->connect(getHostAddresses());

        tick(3 * 3);
    }

    virtual void TearDown() {
        P2pSessionTestFixture::TearDown();

        delete peer3_;
        delete peer2_;
        delete peer1_;
    }

protected:
    void tick(int loop) {
        for (int i = 0; i < loop; ++i) {
            peer1_->tick();
            peer2_->tick();
            peer3_->tick();
            hostSession_->tick();
        }
    }

protected:
    TestP2pEventHandler peerEventHandler1_;
    TestP2pEventHandler peerEventHandler2_;
    TestP2pEventHandler peerEventHandler3_;
    P2pSession* peer1_;
    P2pSession* peer2_;
    P2pSession* peer3_;
};


TEST_F(P2pHostMigrationTest, testConnect)
{
    EXPECT_EQ(3, int(hostSession_->getPeerCount()));
    EXPECT_EQ(3, int(peer1_->getPeerCount()));
    EXPECT_EQ(3, int(peer2_->getPeerCount()));
}


TEST_F(P2pHostMigrationTest, testHostMigration)
{
    hostSession_->close();

    tick(3 * 3);

    EXPECT_EQ(2, int(peer1_->getPeerCount()));
    EXPECT_EQ(2, int(peer2_->getPeerCount()));

    EXPECT_EQ(peerId1_, peerEventHandler1_.getNewHostPeerId()) <<
        "peer1: host migrated";
    EXPECT_EQ(peerId1_, peerEventHandler2_.getNewHostPeerId()) <<
        "peer2: host migrated";
}


TEST_F(P2pHostMigrationTest, testHostMigrationNextPriorityPeer)
{
    peer1_->close();
    hostSession_->close();

    tick(3 * 4);

    EXPECT_EQ(1, int(peer2_->getPeerCount()));

    EXPECT_EQ(peerId2_, peerEventHandler2_.getNewHostPeerId()) <<
        "peer2: host migrated";
}


TEST_F(P2pHostMigrationTest, testDisableHostMigration)
{
    peer1_->close();
    peer2_->close();

    hostSession_->close();
    openHost("", 0, false);

    EXPECT_TRUE(peer1_->open(ACE_DEFAULT_SERVER_PORT + peerId1_));
    EXPECT_TRUE(peer2_->open(ACE_DEFAULT_SERVER_PORT + peerId2_));
    peer1_->connect(getHostAddresses());
    peer2_->connect(getHostAddresses());

    tick(3 * 3);

    hostSession_->close();

    tick(3 * 3);

    EXPECT_EQ(2, int(peer1_->getPeerCount()));
    EXPECT_EQ(2, int(peer2_->getPeerCount()));

    EXPECT_EQ(invalidPeerId, peerEventHandler1_.getNewHostPeerId()) <<
        "peer1: host not migrated";
    EXPECT_EQ(invalidPeerId, peerEventHandler2_.getNewHostPeerId()) <<
        "peer2: host not migrated";
}

