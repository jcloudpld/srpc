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
private:
    virtual void SetUp() {
        SessionTestFixture::SetUp();

        connector_ = new SessionConnector(*sessionManager_);
        pause(10);
    }

    virtual void TearDown() {
        connector_->stop();
        connector_->wait();
        delete connector_;

        SessionTestFixture::TearDown();
    }

protected:
    TestSessionManager* getSessionManager() {
        return static_cast<TestSessionManager*>(sessionManager_);
    }

protected:
    SessionConnector* connector_;
};


TEST_F(SessionConnectorTest, testConnect)
{
    EXPECT_EQ(1,
        connector_->start(getTestAddress(), proactorTask_->getProactor()));

    pause(20);

    EXPECT_EQ(1 * 2, getSessionManager()->getSessionCount());
}


TEST_F(SessionConnectorTest, testMultipleConnect)
{
    EXPECT_EQ(5,
        connector_->start(getTestAddress(), proactorTask_->getProactor(), 5));

    pause(15);

    EXPECT_EQ(5 * 2, getSessionManager()->getSessionCount());
}


TEST_F(SessionConnectorTest, testStopToConnect)
{
    EXPECT_EQ(0,
        connector_->start(getTestAddress(), proactorTask_->getProactor(), 0));

    connector_->stop();

    EXPECT_EQ(0,
        connector_->start(getTestAddress(), proactorTask_->getProactor()));
}
