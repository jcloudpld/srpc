#include "stdafx.h"
#include "P2pRelayTestFixture.h"
#include <nsrpc/utility/SystemUtil.h>
#include <srpc/ContainerTypes.h>

/**
* @class P2pHolePunchingTest
*
* P2P Hole Punching Test
*/
class P2pHolePunchingTest : public P2pRelayTestFixture
{
private:
    virtual void SetUp() {
        P2pRelayTestFixture::SetUp();

        for (int i = 0; i < 4; ++i) {
            hostSession_->tick();
            pause(5);
        }
    }

    //virtual void TearDown();

protected:
    virtual PeerAddresses getHostAddresses() const {
        PeerAddresses hostAddresses;
        const InetAddresses addresses =
            getLocalAddresses(ACE_DEFAULT_SERVER_PORT);
        InetAddresses::const_iterator pos = addresses.begin();
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


TEST_F(P2pHolePunchingTest, testAddressResolving)
{
    EXPECT_TRUE(hostEventHandler_.isAddressResolved());
}


TEST_F(P2pHolePunchingTest, testOnePeerConnectWithAddresses)
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + 1));

    peer->setRelayServer(PeerAddress(ACE_LOCALHOST, relayServerPort));

    for (int i = 0; i < 3; ++i) {
        peer->tick();
        pause(1);
    }

    EXPECT_TRUE(hostEventHandler_.isAddressResolved());
    EXPECT_TRUE(peerEventHandler.isAddressResolved());

    peer->connect(getHostAddresses());

    for (int i = 0; i < (2 * 2); ++i) {
        peer->tick();
        hostSession_->tick();
    }

    EXPECT_EQ(2, hostSession_->getPeerCount());
    EXPECT_EQ(2, peer->getPeerCount());
}


TEST_F(P2pHolePunchingTest, testMultiplePeerConnectWithAddresses)
{
    typedef boost::shared_ptr<P2pSession> P2pSessionPtr;
    typedef std::vector<P2pSessionPtr> P2pSessions;

    const int peerCount = 2;
    P2pSessions peers;
    peers.reserve(peerCount);
    TestP2pEventHandler eventHandlers[peerCount];
    for (int i = 0; i < peerCount; ++i) {
        P2pSessionPtr peer(P2pSessionFactory::create(2 + i, eventHandlers[i]));
        EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + (u_short)i + 1)) <<
            "#" << i << " open";
        peer->setRelayServer(PeerAddress(ACE_LOCALHOST, relayServerPort));
        for (int r = 0; r < 3; ++r) {
            peer->tick();
            pause(1);
        }
        EXPECT_TRUE(eventHandlers[i].isAddressResolved()) <<
            "#" << i << " resolve";
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

    EXPECT_EQ(peerCount + 1, hostSession_->getPeerCount());

    for (int i = 0; i < peerCount; ++i) {
        EXPECT_EQ(peerCount + 1, peers[i]->getPeerCount()) <<
            "#" << i << " peer";
    }
}
