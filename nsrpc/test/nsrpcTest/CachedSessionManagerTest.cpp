#include "stdafx.h"
#include "ProactorSessionTestFixture.h"

#if defined(NSRPC_HAS_PROACTOR)

#include "TestClient.h"
#include "TestCachedSessionManager.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace nsrpc;

const size_t  clientCount = 8;

/**
* @class CachedSessionManagerTest
*
* TestCachedSessionManager Test
*/
class CachedSessionManagerTest : public ProactorSessionTestFixture
{
private:
    virtual void SetUp() {
        ProactorSessionTestFixture::SetUp();

        connect();
    }

    virtual void TearDown() {
        disconnect();

        ProactorSessionTestFixture::TearDown();

        delete sessionFactory_;
    }

    virtual SessionManager* createSessionManager() {
        sessionFactory_ = new RpcProactorSessionFactory(proactorTask_->getProactor());
        return new TestCachedSessionManager(*sessionFactory_);
    }

protected:
    void connect();
    void disconnect();

    TestCachedSessionManager* getSessionManager() {
        return static_cast<TestCachedSessionManager*>(sessionManager_);
    }

protected:
    RpcProactorSessionFactory* sessionFactory_;
    TestClient* client_[clientCount];
};


void CachedSessionManagerTest::connect()
{
    for (size_t i = 0; i < clientCount; ++i) {
        client_[i] = new TestClient;
        EXPECT_TRUE(client_[i]->connect(10, getTestAddress())) <<
            "#" << i << " connect";
    }

    pause(10);
}


void CachedSessionManagerTest::disconnect()
{
    for (size_t i = 0; i < clientCount; ++i) {
        if (client_[i] != 0) {
            client_[i]->close();
            delete client_[i];
            client_[i] = 0;
        }
    }
}


TEST_F(CachedSessionManagerTest, testAcquireAndRelease)
{
    EXPECT_EQ(clientCount, getSessionManager()->getActiveSessionCount());
    EXPECT_EQ(0, int(getSessionManager()->getInactiveSessionCount()));

    disconnect();

    pause(10);

    EXPECT_EQ(0, int(getSessionManager()->getActiveSessionCount()));
    EXPECT_EQ(clientCount, getSessionManager()->getInactiveSessionCount());
}


TEST_F(CachedSessionManagerTest, testCancel)
{
    sessionManager_->cancel();

    EXPECT_EQ(0, int(getSessionManager()->getActiveSessionCount()));
    EXPECT_EQ(clientCount, getSessionManager()->getInactiveSessionCount());
}


TEST_F(CachedSessionManagerTest, testReuseSession)
{
    disconnect();
    connect();
    pause(10);

    EXPECT_EQ(clientCount, getSessionManager()->getActiveSessionCount());
    EXPECT_EQ(0, int(getSessionManager()->getInactiveSessionCount()));
}

#endif // #if defined(NSRPC_HAS_PROACTOR)

