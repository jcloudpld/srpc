#include "stdafx.h"
#include "SessionTestFixture.h"
#include <nsrpc/SessionConnector.h>
#include <nsrpc/utility/SystemUtil.h>

using namespace nsrpc;

/**
* @class SessionConnectorTest
*
* SessionConnector Test
*/
class SessionConnectorTest : public SessionTestFixture
{
    CPPUNIT_TEST_SUITE(SessionConnectorTest);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testMultipleConnect);
    CPPUNIT_TEST(testStopToConnect);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();
private:
    void testConnect();
    void testMultipleConnect();
    void testStopToConnect();
private:
    TestSessionManager* getSessionManager() {
        return static_cast<TestSessionManager*>(sessionManager_);
    }
private:
    SessionConnector* connector_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SessionConnectorTest);

void SessionConnectorTest::setUp()
{
    SessionTestFixture::setUp();

    connector_ = new SessionConnector(*sessionManager_);
    pause(10);
}


void SessionConnectorTest::tearDown()
{
    connector_->stop();
    connector_->wait();
    delete connector_;

    SessionTestFixture::tearDown();
}


void SessionConnectorTest::testConnect()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("connection success",
        1,
        static_cast<int>(connector_->start(getTestAddress(),
            proactorTask_->getProactor())));

    pause(20);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("one session",
        1 * 2, getSessionManager()->getSessionCount());
}


void SessionConnectorTest::testMultipleConnect()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("connection success",
        5,
        static_cast<int>(connector_->start(getTestAddress(),
            proactorTask_->getProactor(), 5)));

    pause(15);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("sessions",
        5 * 2, getSessionManager()->getSessionCount());
}


void SessionConnectorTest::testStopToConnect()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("connection success",
        0,
        static_cast<int>(connector_->start(getTestAddress(),
            proactorTask_->getProactor(), 0)));

    connector_->stop();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("connection failed",
        0,
        static_cast<int>(connector_->start(getTestAddress(),
            proactorTask_->getProactor())));
}
