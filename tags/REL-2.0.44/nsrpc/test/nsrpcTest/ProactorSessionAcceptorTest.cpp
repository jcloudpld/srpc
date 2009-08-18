#include "stdafx.h"
#include "ProactorSessionTestFixture.h"

#if defined(NSRPC_HAS_PROACTOR)

#include "TestClient.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace nsrpc;

/**
* @class ProactorSessionAcceptorTest
*
* ProactorSessionAcceptor Test
*/
class ProactorSessionAcceptorTest : public ProactorSessionTestFixture
{
private:
    virtual void SetUp() {
        ProactorSessionTestFixture::SetUp();

        client_ = new TestClient;
        (void)client_->connect(1, getTestAddress());
    }

    virtual void TearDown() {
        client_->close();
        delete client_;

        ProactorSessionTestFixture::TearDown();
    }

protected:
    TestProactorSessionManager* getSessionManager() {
        return static_cast<TestProactorSessionManager*>(sessionManager_);
    }

protected:
    TestClient* client_;
};


TEST_F(ProactorSessionAcceptorTest, testAccept)
{
    pause(1);

    EXPECT_EQ(1, getSessionManager()->getSessionCount());
}


TEST_F(ProactorSessionAcceptorTest, testMultipleAccept)
{
    const int connectionCount = 5;
    TestClient client[connectionCount];
    for (int i = 0; i < connectionCount; ++i) {
        EXPECT_TRUE(client[i].connect(1, getTestAddress()));
    }

    pause(5);

    EXPECT_EQ(1 + connectionCount, getSessionManager()->getSessionCount());
}


TEST_F(ProactorSessionAcceptorTest, testStopToAccept)
{
    acceptor_->close();

    TestClient client;
    EXPECT_FALSE(client.connect(1, getTestAddress())) << "can't connect";

    EXPECT_FALSE(acceptor_->open(getTestAddress())) << "can't restart";
}


TEST_F(ProactorSessionAcceptorTest, testDisconnected)
{
    pause(1);

    EXPECT_TRUE(getSessionManager()->getSession().isConnected());

    client_->close();

    pause(1);

    EXPECT_EQ(0, getSessionManager()->getSessionCount());
}

#endif // #if defined(NSRPC_HAS_PROACTOR)
