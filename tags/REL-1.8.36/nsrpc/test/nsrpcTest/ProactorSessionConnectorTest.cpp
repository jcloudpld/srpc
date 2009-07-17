#include "stdafx.h"
#include "ProactorSessionTestFixture.h"

#if defined(NSRPC_HAS_PROACTOR)

#include <nsrpc/ProactorSessionConnector.h>
#include <nsrpc/utility/SystemUtil.h>

using namespace nsrpc;

/**
* @class ProactorSessionConnectorTest
*
* ProactorSessionConnector Test
*/
class ProactorSessionConnectorTest : public ProactorSessionTestFixture
{
private:
    virtual void SetUp() {
        ProactorSessionTestFixture::SetUp();

        connector_ = new ProactorSessionConnector(*sessionManager_);
        pause(10);
    }

    virtual void TearDown() {
        connector_->stop();
        connector_->wait();
        delete connector_;

        ProactorSessionTestFixture::TearDown();
    }

protected:
    TestProactorSessionManager* getSessionManager() {
        return static_cast<TestProactorSessionManager*>(sessionManager_);
    }

protected:
    ProactorSessionConnector* connector_;
};


TEST_F(ProactorSessionConnectorTest, testConnect)
{
    EXPECT_EQ(1,
        int(connector_->start(getTestAddress(), proactorTask_->getProactor())));

    pause(20);

    EXPECT_EQ(1 * 2, int(getSessionManager()->getSessionCount()));
}


TEST_F(ProactorSessionConnectorTest, testMultipleConnect)
{
    EXPECT_EQ(5,
        int(connector_->start(getTestAddress(), proactorTask_->getProactor(), 5)));

    pause(15);

    EXPECT_EQ(5 * 2, int(getSessionManager()->getSessionCount()));
}


TEST_F(ProactorSessionConnectorTest, testStopToConnect)
{
    EXPECT_EQ(0,
        int(connector_->start(getTestAddress(), proactorTask_->getProactor(), 0)));

    connector_->stop();

    EXPECT_EQ(0,
        int(connector_->start(getTestAddress(), proactorTask_->getProactor())));
}

#endif // #if defined(NSRPC_HAS_PROACTOR)

