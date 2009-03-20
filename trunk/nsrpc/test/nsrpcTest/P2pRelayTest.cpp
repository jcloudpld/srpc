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
private:
    //virtual void SetUp();
    //virtual void TearDown();

protected:
    virtual PeerAddresses getHostAddresses() const {
        PeerAddresses hostAddresses;
        hostAddresses.push_back(
            PeerAddress("localhost", ACE_DEFAULT_SERVER_PORT - 1));
        return hostAddresses;
    }
};


// ����ȣ��Ʈ ������ �׽�Ʈ�� �Ұ����ϹǷ� �����ϰ� �׽�Ʈ��
TEST_F(P2pRelayTest, testConnectViaRelayServer)
{
    TestP2pEventHandler peerEventHandler;
    boost::scoped_ptr<P2pSession> peer(
        P2pSessionFactory::create(2, peerEventHandler));
    EXPECT_TRUE(peer->open(ACE_DEFAULT_SERVER_PORT + 1));

    peer->setRelayServer(PeerAddress(ACE_LOCALHOST, relayServerPort));

    peer->connect(getHostAddresses());

    for (int i = 0; i < 10; ++i) {
        peer->tick();
        hostSession_->tick();
        pause(1);
    }

    EXPECT_EQ(2, relayService_->getLastRelayPeerIdPair().from_);
    EXPECT_EQ(pseudoHostPeerId, relayService_->getLastRelayPeerIdPair().to_);
}
