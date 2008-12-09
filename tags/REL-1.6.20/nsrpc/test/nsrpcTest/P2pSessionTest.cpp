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
    CPPUNIT_TEST_SUITE(P2pSessionTest);
    CPPUNIT_TEST(testOpen);
    CPPUNIT_TEST(testClose);
    CPPUNIT_TEST(testHost);
    CPPUNIT_TEST(testOnePeerConnect);
    CPPUNIT_TEST(testMultiplePeerConnect);
    CPPUNIT_TEST(testDisconnect);
    CPPUNIT_TEST(testConnectFailed);
    CPPUNIT_TEST(testConnectWithInvalidPassword);
    CPPUNIT_TEST(testConnectWithValidPassword);
    //CPPUNIT_TEST(testLimitPeers);
    //CPPUNIT_TEST(testCannotConnectPeerBeforeHostConnected);
    CPPUNIT_TEST(testBroadcast);
    CPPUNIT_TEST(testUnicast);
    CPPUNIT_TEST_SUITE_END();
public:
    P2pSessionTest();

    void setUp();
    void tearDown();
private:
    void testOpen();
    void testClose();
    void testHost();
    void testOnePeerConnect();
    void testMultiplePeerConnect();
    void testDisconnect();
    void testConnectFailed();
    void testConnectWithInvalidPassword();
    void testConnectWithValidPassword();
    void testLimitPeers();
    void testCannotConnectPeerBeforeHostConnected();
    void testBroadcast();
    void testUnicast();
private:
    size_t getConnectDelay() const {
        return p2pConfig_.connectTimeout_ + (p2pConfig_.connectTimeout_ / 2);
    }
private:
    const P2pConfig p2pConfig_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(P2pSessionTest);

P2pSessionTest::P2pSessionTest() :
    p2pConfig_(nsrpc::P2pConfig::peerDefaultRtt, 80)
{
}


void P2pSessionTest::setUp()
{
    P2pSessionTestFixture::setUp();
}


void P2pSessionTest::tearDown()
{
    P2pSessionTestFixture::tearDown();
}


void P2pSessionTest::testOpen()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("one player",
        1, static_cast<int>(hostSession_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer id",
        1, static_cast<int>(hostSession_->getPeerId()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("sent packets",
        0,
        static_cast<int>(hostSession_->getStats(1).sentReliablePackets_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("recv packets",
        0,
        static_cast<int>(hostSession_->getStats(1).receivedReliablePackets_));
}


void P2pSessionTest::testClose()
{
    hostSession_->close();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("no host",
        false, hostSession_->isHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("no player",
        0, static_cast<int>(hostSession_->getPeerCount()));
}


void P2pSessionTest::testHost()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("host",
        true, hostSession_->isHost());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("one player",
        1, static_cast<int>(hostSession_->getPeerCount()));
}


void P2pSessionTest::testOnePeerConnect()
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer open",
        true, peer->open(ACE_DEFAULT_SERVER_PORT + 1));

    peer->connect(getHostAddresses());

    for (int i = 0; i < (2 * 3); ++i) {
        peer->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host: two peer",
        2, static_cast<int>(hostSession_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer: two peer",
        2, static_cast<int>(peer->getPeerCount()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("the host connected",
        true, peerEventHandler.isConnected(1));

    //const PeerStats statsPeer = hostSession_->getStats(2);
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("peer's sent reliable packets",
    //    1, static_cast<int>(statsPeer.sentReliablePackets_));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("peer's recv reliable packets",
    //    1, static_cast<int>(statsPeer.receivedReliablePackets_));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("peer's sent unreliable packets",
    //    1, static_cast<int>(statsPeer.sentUnreliablePackets_));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("peer's recv unreliable packets",
    //    1, static_cast<int>(statsPeer.receivedUnreliablePackets_));
    //CPPUNIT_ASSERT_MESSAGE(
    //    (boost::format("peer's RTT(%d)") %
    //        static_cast<int>(statsPeer.meanRoundTripTime_)).str(),
    //    5 >= static_cast<int>(statsPeer.meanRoundTripTime_));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("a peer connected",
    //    true, hostEventHandler_.isConnected(2));

    //const PeerStats statsHost = peer->getStats(1);
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("host's reliable sent packets",
    //    1, static_cast<int>(statsHost.sentReliablePackets_));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("host's reliable recv packets",
    //    1, static_cast<int>(statsHost.receivedReliablePackets_));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("host's unreliable sent packets",
    //    1, static_cast<int>(statsHost.sentUnreliablePackets_));
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("host's unreliable recv packets",
    //    1, static_cast<int>(statsHost.receivedUnreliablePackets_));
}


void P2pSessionTest::testMultiplePeerConnect()
{
    typedef boost::shared_ptr<P2pSession> P2pSessionPtr;
    typedef std::vector<P2pSessionPtr> P2pSessions;

    const int peerCount = 3;
    P2pSessions peers;
    peers.reserve(peerCount);
    TestP2pEventHandler eventHandlers[peerCount];
    for (int i = 0; i < peerCount; ++i) {
        P2pSessionPtr peer(P2pSessionFactory::create(2 + i, eventHandlers[i]));
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%u peer") % i).str(),
            true, peer->open(ACE_DEFAULT_SERVER_PORT + (u_short)i + 1));
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

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host",
        peerCount + 1,
        static_cast<int>(hostSession_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("host - connected peer count",
        peerCount, hostEventHandler_.getConnectedPeers());

    for (int i = 0; i < peerCount; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%u peer") % i).str(),
            peerCount + 1,
            static_cast<int>(peers[i]->getPeerCount()));
        const PeerStats stats = peers[i]->getStats(1);
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            (boost::format("#%u peer - connected peer count") % i).str(),
            peerCount, eventHandlers[i].getConnectedPeers());
        //CPPUNIT_ASSERT_MESSAGE(
        //    (boost::format("#%u peer's RTT(%d)") %
        //    i % static_cast<int>(stats.meanRoundTripTime_)).str(),
        //    20 >= static_cast<int>(stats.meanRoundTripTime_));
    }

    //for (size_t i = 0; i < hostSession_->getPeerCount(); ++i) {
    //    PeerStats stats = hostSession_->getStats(static_cast<PeerId>(i + 1));
    //    printf("#%u - (%d,%d,%d,%d)\n", i + 1,
    //        stats.sentReliablePackets_,
    //        stats.receivedReliablePackets_,
    //        stats.sentUnreliablePackets_,
    //        stats.receivedUnreliablePackets_);
    //}
}


void P2pSessionTest::testDisconnect()
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    CPPUNIT_ASSERT_MESSAGE("open",
        peer->open(ACE_DEFAULT_SERVER_PORT + 1));
    peer->connect(getHostAddresses());

    for (int i = 0; i < (2 * 3); ++i) {
        peer->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("two player",
        static_cast<int>(hostSession_->getPeerCount()),
        static_cast<int>(peer->getPeerCount()));

    peer->close();

    for (int i = 0; i < 2 * 3; ++i) {
        peer->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("no peer",
        0, hostEventHandler_.getConnectedPeers());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("only host",
        1, static_cast<int>(hostSession_->getPeerCount()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host only",
        1, peerEventHandler.getConnectedPeers());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("no peer",
        0, static_cast<int>(peer->getPeerCount()));
}


void P2pSessionTest::testConnectFailed()
{
    const P2pConfig p2pConfig(nsrpc::P2pConfig::peerDefaultRtt, 100);

    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler, p2pConfig));
    CPPUNIT_ASSERT_MESSAGE("open",
        peer->open(ACE_DEFAULT_SERVER_PORT + 1));
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

    CPPUNIT_ASSERT_EQUAL_MESSAGE("connect failed",
        pseudoHostPeerId, peerEventHandler.getConnectFailedPeerId());
}


void P2pSessionTest::testConnectWithInvalidPassword()
{
    const srpc::String password("12345");

    hostSession_->close();
    openHost(password);

    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler, p2pConfig_));
    CPPUNIT_ASSERT_MESSAGE("open",
        peer->open(ACE_DEFAULT_SERVER_PORT + 1, "invalid password"));
    peer->connect(getHostAddresses());

    const ACE_Time_Value startTime = ACE_OS::gettimeofday();
    while ((ACE_OS::gettimeofday() - startTime).msec() < getConnectDelay()) {
        peer->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("connect failed",
        pseudoHostPeerId, peerEventHandler.getConnectFailedPeerId());
}


void P2pSessionTest::testConnectWithValidPassword()
{
    const srpc::String password("12345");

    hostSession_->close();
    openHost(password);

    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler, p2pConfig_));
    CPPUNIT_ASSERT_MESSAGE("open",
        peer->open(ACE_DEFAULT_SERVER_PORT + 1, password));
    peer->connect(getHostAddresses());

    const ACE_Time_Value startTime = ACE_OS::gettimeofday();
    while ((ACE_OS::gettimeofday() - startTime).msec() < getConnectDelay()) {
        peer->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("connect succeeded",
        invalidPeerId,
        peerEventHandler.getConnectFailedPeerId());
}


void P2pSessionTest::testLimitPeers()
{
    const size_t maxPeers = 2;

    hostSession_->close();
    openHost("", maxPeers);

    const int peerCount = 2;
    P2pSession* peers[peerCount];
    TestP2pEventHandler eventHandlers[peerCount];
    for (int i = 0; i < peerCount; ++i) {
        peers[i] = P2pSessionFactory::create(2 + i, eventHandlers[i]);
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%u peer") % i).str(),
            true, peers[i]->open(ACE_DEFAULT_SERVER_PORT + (u_short)i + 1));
        peers[i]->connect(getHostAddresses());
    }

    for (int x = 0; x < (peerCount * 8); ++x) {
        for (int i = 0; i < peerCount; ++i) {
            peers[i]->tick();
        }
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host",
        static_cast<int>(maxPeers),
        static_cast<int>(hostSession_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("host - connected peer count",
        static_cast<int>(maxPeers - 1),
        hostEventHandler_.getConnectedPeers());

    for (int i = 0; i < peerCount; ++i) {
        delete peers[i];
    }
}


void P2pSessionTest::testCannotConnectPeerBeforeHostConnected()
{
    TestP2pEventHandler peerEventHandler1;
    boost::scoped_ptr<P2pSession> peer1(
        P2pSessionFactory::create(2, peerEventHandler1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1 open",
        true, peer1->open(ACE_DEFAULT_SERVER_PORT + 1));

    TestP2pEventHandler peerEventHandler2;
    boost::scoped_ptr<P2pSession> peer2(
        P2pSessionFactory::create(3, peerEventHandler2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer2 open",
        true, peer2->open(ACE_DEFAULT_SERVER_PORT + 2));

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

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host: two peer",
        2, static_cast<int>(hostSession_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer1: two peer",
        2, static_cast<int>(peer1->getPeerCount()));
}


void P2pSessionTest::testBroadcast()
{
    TestRpcPlugIn* peerRpcPlugIn1;
    TestP2pEventHandler peerEventHandler1;
    boost::scoped_ptr<P2pSession> peer1(
        P2pSessionFactory::create(2, peerEventHandler1));
    peerRpcPlugIn1 = new TestRpcPlugIn;
    PlugInPtr plugIn1(peerRpcPlugIn1);
    peer1->attach(plugIn1);
    CPPUNIT_ASSERT_MESSAGE("open",
        peer1->open(ACE_DEFAULT_SERVER_PORT + 1));
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
    CPPUNIT_ASSERT_MESSAGE("open",
        peer2->open(ACE_DEFAULT_SERVER_PORT + 2));
    peer2->connect(getHostAddresses());

    for (int i = 0; i < (2 * 4); ++i) {
        peer1->tick();
        peer2->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("two player",
        static_cast<int>(hostSession_->getPeerCount()),
        static_cast<int>(peer1->getPeerCount()));

    hostRpcPlugIn_->hello("hi");

    for (int i = 0; i < (2 * 3); ++i) {
        hostSession_->tick();
        peer1->tick();
        peer2->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("hi",
        std::string("hi"), peerRpcPlugIn1->getLastWorld());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("from host",
        PeerId(1), peerRpcPlugIn1->getLastPeerId());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("hi",
        std::string("hi"), peerRpcPlugIn2->getLastWorld());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("from host",
        PeerId(1), peerRpcPlugIn2->getLastPeerId());
}


void P2pSessionTest::testUnicast()
{
    TestRpcPlugIn* peerRpcPlugIn1;
    TestP2pEventHandler peerEventHandler1;
    boost::scoped_ptr<P2pSession> peer1(
        P2pSessionFactory::create(2, peerEventHandler1));
    peerRpcPlugIn1 = new TestRpcPlugIn;
    PlugInPtr plugIn1(peerRpcPlugIn1);
    peer1->attach(plugIn1);
    CPPUNIT_ASSERT_MESSAGE("open",
        peer1->open(ACE_DEFAULT_SERVER_PORT + 1));
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
    CPPUNIT_ASSERT_MESSAGE("open",
        peer2->open(ACE_DEFAULT_SERVER_PORT + 2));
    peer2->connect(getHostAddresses());

    for (int i = 0; i < (2 * 4); ++i) {
        peer1->tick();
        peer2->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("two player",
        static_cast<int>(hostSession_->getPeerCount()),
        static_cast<int>(peer1->getPeerCount()));

    const PeerHint hint(2);
    hostRpcPlugIn_->hello("hi", &hint);

    for (int i = 0; i < (2 * 3); ++i) {
        hostSession_->tick();
        peer1->tick();
        peer2->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("hi",
        std::string("hi"), peerRpcPlugIn1->getLastWorld());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("from host",
        PeerId(1), peerRpcPlugIn1->getLastPeerId());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("hi",
        std::string(""), peerRpcPlugIn2->getLastWorld());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("from host",
        invalidPeerId, peerRpcPlugIn2->getLastPeerId());
}
