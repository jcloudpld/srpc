#include "stdafx.h"
#include "P2pRelayTestFixture.h"
#include <nsrpc/utility/SystemUtil.h>

/**
* @class P2pHolePunchingTest
*
* P2P Hole Punching Test
*/
class P2pHolePunchingTest : public P2pRelayTestFixture
{
    CPPUNIT_TEST_SUITE(P2pHolePunchingTest);
    CPPUNIT_TEST(testAddressResolving);
    CPPUNIT_TEST(testOnePeerConnectWithAddresses);
    CPPUNIT_TEST(testMultiplePeerConnectWithAddresses);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testAddressResolving();
    void testOnePeerConnectWithAddresses();
    void testMultiplePeerConnectWithAddresses();
private:
    virtual PeerAddresses getHostAddresses() const {
        PeerAddresses hostAddresses;
        const std::vector<ACE_INET_Addr> addresses =
            getLocalAddresses(ACE_DEFAULT_SERVER_PORT);
        std::vector<ACE_INET_Addr>::const_iterator pos = addresses.begin();
        for (; pos != addresses.end(); ++pos) {
            const ACE_INET_Addr& addr = *pos;
            hostAddresses.push_back(
                PeerAddress(addr.get_host_addr(), addr.get_port_number()));
        }
        PeerAddresses defaultHostAddresses =
            P2pRelayTestFixture::getHostAddresses();
        hostAddresses.insert(hostAddresses.end(),
            defaultHostAddresses.begin(), defaultHostAddresses.end());
        return hostAddresses;
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(P2pHolePunchingTest);

void P2pHolePunchingTest::setUp()
{
    P2pRelayTestFixture::setUp();

    for (int i = 0; i < 4; ++i) {
        hostSession_->tick();
        pause(5);
    }
}


void P2pHolePunchingTest::tearDown()
{
    P2pRelayTestFixture::tearDown();
}


void P2pHolePunchingTest::testAddressResolving()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("address resolved",
        true, hostEventHandler_.isAddressResolved());
}


void P2pHolePunchingTest::testOnePeerConnectWithAddresses()
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer open",
        true, peer->open(ACE_DEFAULT_SERVER_PORT + 1));

    peer->setRelayServer(PeerAddress(ACE_LOCALHOST, relayServerPort));

    for (int i = 0; i < 3; ++i) {
        peer->tick();
        pause(1);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("address resolved",
        true, hostEventHandler_.isAddressResolved());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("address resolved",
        true, peerEventHandler.isAddressResolved());

    peer->connect(getHostAddresses());

    for (int i = 0; i < (2 * 2); ++i) {
        peer->tick();
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host: two peer",
        2, static_cast<int>(hostSession_->getPeerCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer: two peer",
        2, static_cast<int>(peer->getPeerCount()));
}


void P2pHolePunchingTest::testMultiplePeerConnectWithAddresses()
{
    typedef boost::shared_ptr<P2pSession> P2pSessionPtr;
    typedef std::vector<P2pSessionPtr> P2pSessions;

    const int peerCount = 2;
    P2pSessions peers;
    peers.reserve(peerCount);
    TestP2pEventHandler eventHandlers[peerCount];
    for (int i = 0; i < peerCount; ++i) {
        P2pSessionPtr peer(P2pSessionFactory::create(2 + i, eventHandlers[i]));
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%u open") % i).str(),
            true, peer->open(ACE_DEFAULT_SERVER_PORT + (u_short)i + 1));
        peer->setRelayServer(PeerAddress(ACE_LOCALHOST, relayServerPort));
        for (int r = 0; r < 3; ++r) {
            peer->tick();
            pause(1);
        }
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%u resolve") % i).str(),
            true, eventHandlers[i].isAddressResolved());
        peer->connect(getHostAddresses());
        peer->tick();
        peers.push_back(peer);
    }

    for (int x = 0; x < peerCount * 9; ++x) {
        for (int i = 0; i < peerCount; ++i) {
            peers[i]->tick();
        }
        hostSession_->tick();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("host",
        peerCount + 1,
        static_cast<int>(hostSession_->getPeerCount()));

    for (int i = 0; i < peerCount; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%u peer") % i).str(),
            peerCount + 1,
            static_cast<int>(peers[i]->getPeerCount()));
    }
}
