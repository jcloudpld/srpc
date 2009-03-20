#include "stdafx.h"
#include "SessionTestFixture.h"
#include "TestClient.h"
#include "TestCachedSessionManager.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace nsrpc;

/**
* @class CachedSessionManagerTest
*
* TestCachedSessionManager Test
*/
class CachedSessionManagerTest : public SessionTestFixture
{
private:
    virtual void SetUp() {
        SessionTestFixture::SetUp();

        connect();
    }

    virtual void TearDown() {
        disconnect();

        SessionTestFixture::TearDown();

        delete sessionFactory_;
    }

    virtual SessionManager* createSessionManager() {
        sessionFactory_ = new RpcSessionFactory(proactorTask_->getProactor());
        return new TestCachedSessionManager(*sessionFactory_);
    }

protected:
    void connect();
    void disconnect();

    TestCachedSessionManager* getSessionManager() {
        return static_cast<TestCachedSessionManager*>(sessionManager_);
    }

protected:
    enum {
        clientCount = 8
    };

    RpcSessionFactory* sessionFactory_;
    TestClient* client_[clientCount];
};


void CachedSessionManagerTest::connect()
{
    for (int i = 0; i < clientCount; ++i) {
        client_[i] = new TestClient;
        EXPECT_TRUE(client_[i]->connect(10, getTestAddress())) <<
            "#" << i << " connect";
    }

    pause(10);
}


void CachedSessionManagerTest::disconnect()
{
    for (int i = 0; i < clientCount; ++i) {
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
    EXPECT_EQ(0, getSessionManager()->getInactiveSessionCount());

    disconnect();

    pause(10);

    EXPECT_EQ(0, getSessionManager()->getActiveSessionCount());
    EXPECT_EQ(clientCount, getSessionManager()->getInactiveSessionCount());
}


TEST_F(CachedSessionManagerTest, testCancel)
{
    sessionManager_->cancel();

    EXPECT_EQ(0, getSessionManager()->getActiveSessionCount());
    EXPECT_EQ(clientCount, getSessionManager()->getInactiveSessionCount());
}


TEST_F(CachedSessionManagerTest, testReuseSession)
{
    disconnect();
    connect();
    pause(10);

    EXPECT_EQ(clientCount, getSessionManager()->getActiveSessionCount());
    EXPECT_EQ(0, getSessionManager()->getInactiveSessionCount());
}
