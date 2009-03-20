#include "stdafx.h"
#include "AceTestFixture.h"
#include <nsrpc/utility/LogManager.h>
#include <ace/Log_Msg_Callback.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4244)
#endif
#include <ace/Log_Record.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

using namespace nsrpc;

/**
* @class AceLogManagerTest
*
* LogManager Test
*/
class AceLogManagerTest : public AceTestFixture
{
private:
    virtual void SetUp() {
        AceTestFixture::SetUp();

        repository_ = LogRepositoryFactory::create(false);
    }

    virtual void TearDown() {
        nsrpc::LogManager::redirectToStderr();
        delete repository_;

        AceTestFixture::TearDown();
    }

protected:
    LogRepository* repository_;
};


TEST_F(AceLogManagerTest, testCallback)
{
    nsrpc::LogManager::unsilent();
    nsrpc::LogManager::redirectToCallback(*repository_);

    const srpc::String logString("Hello, World!!");
    int aceError = ACE_Log_Msg::last_error_adapter();
    ACE_Log_Msg *ace = ACE_Log_Msg::instance();
    ace->conditional_set(__FILE__, __LINE__, 0, aceError);
    ace->log(LM_DEBUG, logString.c_str());

    EXPECT_TRUE(repository_->hasLog());

    EXPECT_EQ(logString, repository_->getLog());
}


TEST_F(AceLogManagerTest, testCallbackWithEventHandler)
{
    class DummyLogEventHandler : public nsrpc::LogEventHandler
    {
    public:
        virtual void onLog(const char* log) {
            log_ = log;
        }

        const srpc::String& getLog() const {
            return log_;
        }
    private:
        srpc::String log_;
    };

    DummyLogEventHandler logEventHandler;
    boost::scoped_ptr<LogRepository> repository(
        LogRepositoryFactory::create(false, &logEventHandler));

    nsrpc::LogManager::unsilent();
    nsrpc::LogManager::redirectToCallback(*repository);

    const srpc::String logString("Hello, World!!");
    int aceError = ACE_Log_Msg::last_error_adapter();
    ACE_Log_Msg *ace = ACE_Log_Msg::instance();
    ace->conditional_set(__FILE__, __LINE__, 0, aceError);
    ace->log(LM_DEBUG, logString.c_str());

    EXPECT_EQ(logString, logEventHandler.getLog());
}
