#include "stdafx.h"
#include "SessionTestFixture.h"

#if defined(NSRPC_HAS_PROACTOR)

#include "TestClient.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace srpc;
using namespace nsrpc;

/**
* @class SessionTest
*
* Session Test
*/
class SessionTest : public SessionTestFixture
{
private:
    virtual void SetUp() {
        SessionTestFixture::SetUp();

        client_ = new TestClient;
        (void)client_->connect(1, getTestAddress());
    }

    virtual void TearDown() {
        client_->close();
        delete client_;

        SessionTestFixture::TearDown();
    }

protected:
    TestSession& getLastSession() {
        return static_cast<TestSessionManager*>(sessionManager_)->getSession();
    }

protected:
    TestClient* client_;
};


TEST_F(SessionTest, testRecvPackets)
{
    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        client_->sendMessage(body, bodySize);
    }

    pause(1);

    EXPECT_EQ((packetCoder_->getHeaderSize() + bodySize) * sendCount,
        getLastSession().getStats().recvBytes_);
    EXPECT_EQ(sendCount, int(getLastSession().getArrivedMessageCount()));

    // ?Ʒ? ?׽?Ʈ?? ū ?ǹ̰? ??��
    EXPECT_EQ(0,
        int(getLastSession().getAcquireSendBlockCallCount())) <<
        "acquireSendBlock call count";
}


TEST_F(SessionTest, testSendPackets)
{
    pause(1);

    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        ACE_Message_Block& mblock = getLastSession().acquireSendBlock();
        const void* bodyPtr = body;
        mblock.copy(static_cast<const char*>(bodyPtr), bodySize);
        getLastSession().sendMessage(mblock);
    }

    getLastSession().disconnectGracefully();

    pause(1);

    for (int i = 0; i < sendCount; ++i) {
        UInt8 received[bodySize];
        EXPECT_EQ(bodySize, client_->recvMessage(received, bodySize)) <<
            "received size";
        EXPECT_TRUE(memcmp(body, received, bodySize) == 0);
    }

    EXPECT_EQ((packetCoder_->getHeaderSize() + bodySize) * sendCount,
        getLastSession().getStats().sentBytes_) << "sent bytes";

    EXPECT_EQ(sendCount,
	    int(getLastSession().getAcquireSendBlockCallCount())) <<
        "acquireSendBlock call count";
}


TEST_F(SessionTest, testConnect)
{
    TestSession* session =
        static_cast<TestSession*>(sessionManager_->acquire());

    EXPECT_TRUE(
        session->connect(getTestAddress().get_host_name(),
            getTestAddress().get_port_number(), 10));

    pause(1);

    const char body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;
    ACE_Message_Block& mblock = getLastSession().acquireSendBlock();
    mblock.copy(body, bodySize);
    getLastSession().sendMessage(mblock);

    pause(1);

    EXPECT_EQ(1, int(session->getArrivedMessageCount())) <<
        "received count";

    sessionManager_->release(session);
}

#endif // #if defined(NSRPC_HAS_PROACTOR)

