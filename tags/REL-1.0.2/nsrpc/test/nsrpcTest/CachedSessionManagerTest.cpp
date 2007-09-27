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
    CPPUNIT_TEST_SUITE(CachedSessionManagerTest);
    CPPUNIT_TEST(testAcquireAndRelease);
    CPPUNIT_TEST(testCancel);
    CPPUNIT_TEST(testReuseSession);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();

    virtual SessionManager* createSessionManager() {
        sessionFactory_ = new RpcSessionFactory(proactorTask_->getProactor());
        return new TestCachedSessionManager(*sessionFactory_);
    }
private:
    void testAcquireAndRelease();
    void testCancel();
    void testReuseSession();
private:
    void connect();
    void disconnect();

    TestCachedSessionManager* getSessionManager() {
        return static_cast<TestCachedSessionManager*>(sessionManager_);
    }
private:
    enum {
        clientCount = 8
    };

    RpcSessionFactory* sessionFactory_;
    TestClient* client_[clientCount];
};

CPPUNIT_TEST_SUITE_REGISTRATION(CachedSessionManagerTest);

void CachedSessionManagerTest::setUp()
{
    SessionTestFixture::setUp();

    connect();
}


void CachedSessionManagerTest::tearDown()
{
    disconnect();

    SessionTestFixture::tearDown();

    delete sessionFactory_;
}


void CachedSessionManagerTest::connect()
{
    for (int i = 0; i < clientCount; ++i) {
        client_[i] = new TestClient;
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%u connect") % i).str(),
            true, client_[i]->connect(10, getTestAddress()));
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


void CachedSessionManagerTest::testAcquireAndRelease()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("active session count",
        static_cast<int>(clientCount),
        static_cast<int>(getSessionManager()->getActiveSessionCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive session count",
        0,
        static_cast<int>(getSessionManager()->getInactiveSessionCount()));

    disconnect();

    pause(10);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active session count",
        0,
        static_cast<int>(getSessionManager()->getActiveSessionCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive session count",
        static_cast<int>(clientCount),
        static_cast<int>(getSessionManager()->getInactiveSessionCount()));
}


void CachedSessionManagerTest::testCancel()
{
    sessionManager_->cancel();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active session count",
        0,
        static_cast<int>(getSessionManager()->getActiveSessionCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive session count",
        static_cast<int>(clientCount),
        static_cast<int>(getSessionManager()->getInactiveSessionCount()));
}


void CachedSessionManagerTest::testReuseSession()
{
    disconnect();
    connect();
    pause(10);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active session count",
        static_cast<int>(clientCount),
        static_cast<int>(getSessionManager()->getActiveSessionCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive session count",
        0,
        static_cast<int>(getSessionManager()->getInactiveSessionCount()));
}
