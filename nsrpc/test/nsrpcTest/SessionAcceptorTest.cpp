#include "stdafx.h"
#include "SessionTestFixture.h"
#include "TestClient.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace nsrpc;

/**
* @class SessionAcceptorTest
*
* SessionAcceptor Test
*/
class SessionAcceptorTest : public SessionTestFixture
{
    CPPUNIT_TEST_SUITE(SessionAcceptorTest);
    CPPUNIT_TEST(testAccept);
    CPPUNIT_TEST(testMultipleAccept);
    CPPUNIT_TEST(testStopToAccept);
    CPPUNIT_TEST(testDisconnected);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();
private:
    void testAccept();
    void testMultipleAccept();
    void testStopToAccept();
    void testDisconnected();
private:
    TestSessionManager* getSessionManager() {
        return static_cast<TestSessionManager*>(sessionManager_);
    }
private:
    TestClient* client_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SessionAcceptorTest);

void SessionAcceptorTest::setUp()
{
    SessionTestFixture::setUp();

    client_ = new TestClient;
    (void)client_->connect(1, getTestAddress());
}


void SessionAcceptorTest::tearDown()
{
    client_->close();
    delete client_;

    SessionTestFixture::tearDown();
}


void SessionAcceptorTest::testAccept()
{
    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("a session",
        1, getSessionManager()->getSessionCount());
}


void SessionAcceptorTest::testMultipleAccept()
{
    const int connectionCount = 5;
    TestClient client[connectionCount];
    for (int i = 0; i < connectionCount; ++i) {
        CPPUNIT_ASSERT_EQUAL_MESSAGE("connect",
            true, client[i].connect(1, getTestAddress()));
    }

    pause(5);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("sessions",
        1 + connectionCount, getSessionManager()->getSessionCount());
}


void SessionAcceptorTest::testStopToAccept()
{
    acceptor_->close();

    TestClient client;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("can't connect",
        false, client.connect(1, getTestAddress()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("can't restart",
        false, acceptor_->open(getTestAddress()));
}


void SessionAcceptorTest::testDisconnected()
{
    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("connected",
        true, getSessionManager()->getSession().isConnected());

    client_->close();

    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("disconnected",
        0, getSessionManager()->getSessionCount());
}
