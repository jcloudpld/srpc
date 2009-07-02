#include "stdafx.h"
#include "ReactorSessionTestFixture.h"

#if defined(NSRPC_HAS_PROACTOR)

#include "TestClient.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace srpc;
using namespace nsrpc;

/**
* @class ClientSessionTest
*
* Session Test
*/
class ClientSessionTest : public ReactorSessionTestFixture
{
private:
    virtual void SetUp() {
        ReactorSessionTestFixture::SetUp();

        clientSession_->connect(getTestAddress().get_host_addr(),
            getTestAddress().get_port_number(), 1);

        pause(1);
    }

    virtual void TearDown() {
        clientSession_->disconnect();

        ReactorSessionTestFixture::TearDown();
    }

protected:
    TestProactorSessionManager* getSessionManager() {
        return static_cast<TestProactorSessionManager*>(sessionManager_);
    }

    TestProactorSession& getLastServerSession() {
        return getSessionManager()->getSession();
    }
};


TEST_F(ClientSessionTest, testConnect)
{
    EXPECT_EQ(1, getSessionManager()->getSessionCount());
}


TEST_F(ClientSessionTest, testDisconnected)
{
    getLastServerSession().disconnect();

    pause(1);

    EXPECT_FALSE(clientSession_->isConnected()) << "disconnected";
    EXPECT_TRUE(clientSession_->isDisconnected()) << "disconnected";
    //EXPECT_EQ(0, getSessionManager()->getSessionCount());
}


TEST_F(ClientSessionTest, testSendPackets)
{
    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        ACE_Message_Block& mblock = clientSession_->acquireSendBlock();
        const void* bodyPtr = body;
        mblock.copy(static_cast<const char*>(bodyPtr), bodySize);
        clientSession_->sendMessage(mblock, mtGeneral);
    }

    pause(1);

    EXPECT_EQ((packetCoder_->getHeaderSize() + bodySize) * sendCount,
        getLastServerSession().getStats().recvBytes_) << "received bytes";
    EXPECT_EQ(sendCount,
	    int(getLastServerSession().getArrivedMessageCount())) <<
        "received count";
}


TEST_F(ClientSessionTest, testRecvPackets)
{
    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        ACE_Message_Block& mblock = getLastServerSession().acquireSendBlock();
        const void* bodyPtr = body;
        mblock.copy(static_cast<const char*>(bodyPtr), bodySize);
        getLastServerSession().sendMessage(mblock);
    }

    pause(1);

    EXPECT_EQ((packetCoder_->getHeaderSize() + bodySize) * sendCount,
        getLastServerSession().getStats().sentBytes_) << "sent bytes";
    EXPECT_EQ(sendCount,
		int(clientSession_->getMessageCount())) << "message count";
}

#endif // #if defined(NSRPC_HAS_PROACTOR)

