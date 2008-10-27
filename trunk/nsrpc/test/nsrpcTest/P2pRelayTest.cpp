#include "stdafx.h"
#include "P2pRelayTestFixture.h"
#include <nsrpc/utility/SystemUtil.h>

/**
* @class P2pRelayTest
*
* P2P Relaying Test
*/
class P2pRelayTest : public P2pRelayTestFixture
{
    CPPUNIT_TEST_SUITE(P2pRelayTest);
    CPPUNIT_TEST(testConnectViaRelayServer);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testConnectViaRelayServer();
private:
    virtual PeerAddresses getHostAddresses() const {
        PeerAddresses hostAddresses;
        hostAddresses.push_back(
            PeerAddress("localhost", ACE_DEFAULT_SERVER_PORT - 1));
        return hostAddresses;
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(P2pRelayTest);

void P2pRelayTest::setUp()
{
    P2pRelayTestFixture::setUp();
}


void P2pRelayTest::tearDown()
{
    P2pRelayTestFixture::tearDown();
}


// 로컬호스트 내에서 테스트가 불가능하므로 간단하게 테스트함
void P2pRelayTest::testConnectViaRelayServer()
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("peer open",
        true, peer->open(ACE_DEFAULT_SERVER_PORT + 1));

    peer->setRelayServer(PeerAddress(ACE_LOCALHOST, relayServerPort));

    peer->connect(getHostAddresses());

    for (int i = 0; i < 10; ++i) {
        peer->tick();
        hostSession_->tick();
        pause(1);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("from",
        2, static_cast<int>(relayService_->getLastRelayPeerIdPair().from_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("to",
        pseudoHostPeerId, relayService_->getLastRelayPeerIdPair().to_);
}
