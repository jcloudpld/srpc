#include "stdafx.h"
#include "P2pSessionTestFixture.h"
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/p2p/PeerHint.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/SystemUtil.h>

/**
* @class P2pSessionTest
*
* P2P P2pSession Test
*/
class P2pSessionTest : public P2pSessionTestFixture
{
public:
    P2pSessionTest() :
        p2pConfig_(nsrpc::P2pConfig::peerDefaultRtt, 80) {}

private:
    //virtual void SetUp();
    //virtual void TearDown();

protected:
    size_t getConnectDelay() const {
        return p2pConfig_.connectTimeout_ + (p2pConfig_.connectTimeout_ / 2);
    }

protected:
    const P2pConfig p2pConfig_;
};


TEST_F(P2pSessionTest, testOpen)
{
    EXPECT_EQ(1, hostSession_->getPeerCount());
    EXPECT_EQ(1, hostSession_->getPeerId());

    EXPECT_EQ(0, hostSession_->getStats(1).sentReliablePackets_);
    EXPECT_EQ(0, hostSession_->getStats(1).receivedReliablePackets_);
}


TEST_F(P2pSessionTest, testClose)
{
    hostSession_->close();

    EXPECT_FALSE(hostSession_->isHost());
    EXPECT_EQ(0, int(hostSession_->getPeerCount()));
}


TEST_F(P2pSessionTest, testHost)
{
    EXPECT_TRUE(hostSession_->isHost());
    EXPECT_EQ(1, hostSession_->getPeerCount());
}


TEST_F(P2pSessionTest, testOnePeerConnect)
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + 1));

    peer->connect(getHostAddresses());

    for (int i = 0; i < (2 * 3); ++i) {
        peer->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(2, hostSession_->getPeerCount()) << "host: two peer";
    EXPECT_EQ(2, peer->getPeerCount()) << "peer: two peer";

    EXPECT_TRUE(peerEventHandler.isConnected(1)) << "the host connected";
}


TEST_F(P2pSessionTest, testMultiplePeerConnect)
{
    typedef boost::shared_ptr<P2pSession> P2pSessionPtr;
    typedef std::vector<P2pSessionPtr> P2pSessions;

    const int peerCount = 3;
    P2pSessions peers;
    peers.reserve(peerCount);
    TestP2pEventHandler eventHandlers[peerCount];
    for (int i = 0; i < peerCount; ++i) {
        P2pSessionPtr peer(P2pSessionFactory::create(2 + i, eventHandlers[i]));
        EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + (u_short)i + 1)) <<
            "#" << i << " peer";
        peer->connect(getHostAddresses());
        peer->tick();
        peers.push_back(peer);
    }

    for (int x = 0; x < (peerCount * 8); ++x) {
        for (int i = 0; i < peerCount; ++i) {
            peers[i]->tick();
        }
        hostSession_->tick();
        pause(1);
    }

    EXPECT_EQ(peerCount + 1, hostSession_->getPeerCount());
    EXPECT_EQ(peerCount, hostEventHandler_.getConnectedPeers()) <<
        "host - connected peer count";

    for (int i = 0; i < peerCount; ++i) {
        EXPECT_EQ(peerCount + 1, peers[i]->getPeerCount()) <<
            "#" << i << " peer";
        const PeerStats stats = peers[i]->getStats(1);
        EXPECT_EQ(peerCount, eventHandlers[i].getConnectedPeers()) <<
            "#" << i << " peer - connected peer count";
    }
}


TEST_F(P2pSessionTest, testDisconnect)
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + 1));
    peer->connect(getHostAddresses());

    for (int i = 0; i < (2 * 3); ++i) {
        peer->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(hostSession_->getPeerCount(), peer->getPeerCount());

    peer->close();

    for (int i = 0; i < 2 * 3; ++i) {
        peer->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(0, hostEventHandler_.getConnectedPeers());
    EXPECT_EQ(1, hostSession_->getPeerCount());

    EXPECT_EQ(1, peerEventHandler.getConnectedPeers());
    EXPECT_EQ(0, int(peer->getPeerCount()));
}


TEST_F(P2pSessionTest, testConnectFailed)
{
    const P2pConfig p2pConfig(nsrpc::P2pConfig::peerDefaultRtt, 100);

    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler, p2pConfig));
    EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + 1));
    PeerAddresses invalidAddresses;
    invalidAddresses.push_back(
        PeerAddress(ACE_LOCALHOST, ACE_DEFAULT_SERVER_PORT + 3));
    peer->connect(invalidAddresses);

    const ACE_Time_Value startTime = ACE_OS::gettimeofday();
    while ((ACE_OS::gettimeofday() - startTime).msec() < getConnectDelay()) {
        peer->tick();
        hostSession_->tick();

        if (pseudoHostPeerId == peerEventHandler.getConnectFailedPeerId()) {
            break;
        }
    }

    EXPECT_EQ(pseudoHostPeerId, peerEventHandler.getConnectFailedPeerId()) <<
        "connect failed";
}


TEST_F(P2pSessionTest, testConnectWithInvalidPassword)
{
    const srpc::String password("12345");

    hostSession_->close();
    openHost(password);

    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler, p2pConfig_));
    EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + 1, "invalid password"));
    peer->connect(getHostAddresses());

    const ACE_Time_Value startTime = ACE_OS::gettimeofday();
    while ((ACE_OS::gettimeofday() - startTime).msec() < getConnectDelay()) {
        peer->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(pseudoHostPeerId, peerEventHandler.getConnectFailedPeerId()) <<
        "connect failed";
}


TEST_F(P2pSessionTest, testConnectWithValidPassword)
{
    const srpc::String password("12345");

    hostSession_->close();
    openHost(password);

    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler, p2pConfig_));
    EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + 1, password));
    peer->connect(getHostAddresses());

    const ACE_Time_Value startTime = ACE_OS::gettimeofday();
    while ((ACE_OS::gettimeofday() - startTime).msec() < getConnectDelay()) {
        peer->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(invalidPeerId, peerEventHandler.getConnectFailedPeerId()) <<
        "connect succeeded";
}


TEST_F(P2pSessionTest, testLimitPeers)
{
    const size_t maxPeers = 2;

    hostSession_->close();
    openHost("", maxPeers);

    const int peerCount = 2;
    P2pSession* peers[peerCount];
    TestP2pEventHandler eventHandlers[peerCount];
    for (int i = 0; i < peerCount; ++i) {
        peers[i] = P2pSessionFactory::create(2 + i, eventHandlers[i]);
        EXPECT_TRUE(peers[i]->open(ACE_DEFAULT_SERVER_PORT + (u_short)i + 1)) <<
            "#" << i << "peer";
        peers[i]->connect(getHostAddresses());
    }

    for (int x = 0; x < (peerCount * 8); ++x) {
        for (int i = 0; i < peerCount; ++i) {
            peers[i]->tick();
        }
        hostSession_->tick();
    }

    EXPECT_EQ(maxPeers, hostSession_->getPeerCount());
    EXPECT_EQ(maxPeers - 1, hostEventHandler_.getConnectedPeers());

    for (int i = 0; i < peerCount; ++i) {
        delete peers[i];
    }
}


TEST_F(P2pSessionTest, testCannotConnectPeerBeforeHostConnected)
{
    TestP2pEventHandler peerEventHandler1;
    boost::scoped_ptr<P2pSession> peer1(
        P2pSessionFactory::create(2, peerEventHandler1));
    EXPECT_TRUE(peer1->open(ACE_DEFAULT_SERVER_PORT + 1));

    TestP2pEventHandler peerEventHandler2;
    boost::scoped_ptr<P2pSession> peer2(
        P2pSessionFactory::create(3, peerEventHandler2));
    EXPECT_TRUE(peer2->open(ACE_DEFAULT_SERVER_PORT + 2));

    peer1->connect(getHostAddresses());

    for (int i = 0; i < (2 * 3); ++i) {
        peer1->tick();
        hostSession_->tick();
    }

    peer2->connect(peer1->getAddresses(2));

    for (int i = 0; i < (2 * 3); ++i) {
        peer1->tick();
        peer2->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(2, hostSession_->getPeerCount());
    EXPECT_EQ(2, peer1->getPeerCount());
}


TEST_F(P2pSessionTest, testBroadcast)
{
    TestRpcPlugIn* peerRpcPlugIn1;
    TestP2pEventHandler peerEventHandler1;
    boost::scoped_ptr<P2pSession> peer1(
        P2pSessionFactory::create(2, peerEventHandler1));
    peerRpcPlugIn1 = new TestRpcPlugIn;
    PlugInPtr plugIn1(peerRpcPlugIn1);
    peer1->attach(plugIn1);
    EXPECT_TRUE(peer1->open(ACE_DEFAULT_SERVER_PORT + 1));
    peer1->connect(getHostAddresses());

    for (int i = 0; i < (2 * 3); ++i) {
        peer1->tick();
        hostSession_->tick();
    }

    TestRpcPlugIn* peerRpcPlugIn2;
    TestP2pEventHandler peerEventHandler2;
    boost::scoped_ptr<P2pSession> peer2(
        P2pSessionFactory::create(3, peerEventHandler2));
    peerRpcPlugIn2 = new TestRpcPlugIn;
    PlugInPtr plugIn2(peerRpcPlugIn2);
    peer2->attach(plugIn2);
    EXPECT_TRUE(peer2->open(ACE_DEFAULT_SERVER_PORT + 2));
    peer2->connect(getHostAddresses());

    for (int i = 0; i < (2 * 4); ++i) {
        peer1->tick();
        peer2->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(hostSession_->getPeerCount(), peer1->getPeerCount());

    hostRpcPlugIn_->hello("hi");

    for (int i = 0; i < (2 * 4); ++i) {
        hostSession_->tick();
        peer1->tick();
        peer2->tick();
    }

    EXPECT_EQ("hi", peerRpcPlugIn1->getLastWorld());
    EXPECT_EQ(PeerId(1), peerRpcPlugIn1->getLastPeerId());

    EXPECT_EQ("hi", peerRpcPlugIn2->getLastWorld());
    EXPECT_EQ(PeerId(1), peerRpcPlugIn2->getLastPeerId());
}


TEST_F(P2pSessionTest, testUnicast)
{
    TestRpcPlugIn* peerRpcPlugIn1;
    TestP2pEventHandler peerEventHandler1;
    boost::scoped_ptr<P2pSession> peer1(
        P2pSessionFactory::create(2, peerEventHandler1));
    peerRpcPlugIn1 = new TestRpcPlugIn;
    PlugInPtr plugIn1(peerRpcPlugIn1);
    peer1->attach(plugIn1);
    EXPECT_TRUE(peer1->open(ACE_DEFAULT_SERVER_PORT + 1));
    peer1->connect(getHostAddresses());

    for (int i = 0; i < (2 * 3); ++i) {
        peer1->tick();
        hostSession_->tick();
    }

    TestRpcPlugIn* peerRpcPlugIn2;
    TestP2pEventHandler peerEventHandler2;
    boost::scoped_ptr<P2pSession> peer2(
        P2pSessionFactory::create(3, peerEventHandler2));
    peerRpcPlugIn2 = new TestRpcPlugIn;
    PlugInPtr plugIn2(peerRpcPlugIn2);
    peer2->attach(plugIn2);
    EXPECT_TRUE(peer2->open(ACE_DEFAULT_SERVER_PORT + 2));
    peer2->connect(getHostAddresses());

    for (int i = 0; i < (2 * 4); ++i) {
        peer1->tick();
        peer2->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(hostSession_->getPeerCount(), peer1->getPeerCount());

    const PeerHint hint(2);
    hostRpcPlugIn_->hello("hi", &hint);

    for (int i = 0; i < (2 * 3); ++i) {
        hostSession_->tick();
        peer1->tick();
        peer2->tick();
    }

    EXPECT_EQ("hi", peerRpcPlugIn1->getLastWorld());
    EXPECT_EQ(PeerId(1), peerRpcPlugIn1->getLastPeerId());

    EXPECT_EQ("", peerRpcPlugIn2->getLastWorld());
    EXPECT_EQ(invalidPeerId, peerRpcPlugIn2->getLastPeerId());
}
