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
    CPPUNIT_TEST_SUITE(AceLogManagerTest);
    CPPUNIT_TEST(testCallback);
    CPPUNIT_TEST(testCallbackWithEventHandler);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();
private:
    void testCallback();
    void testCallbackWithEventHandler();
private:
    LogRepository* repository_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AceLogManagerTest);

void AceLogManagerTest::setUp()
{
    repository_ = LogRepositoryFactory::create(false);
}


void AceLogManagerTest::tearDown()
{
    nsrpc::LogManager::redirectToStderr();

    delete repository_;
}


void AceLogManagerTest::testCallback()
{
    nsrpc::LogManager::unsilent();
    nsrpc::LogManager::redirectToCallback(*repository_);

    const std::string logString("Hello, World!!");
    int aceError = ACE_Log_Msg::last_error_adapter();
    ACE_Log_Msg *ace = ACE_Log_Msg::instance();
    ace->conditional_set(__FILE__, __LINE__, 0, aceError);
    ace->log(LM_DEBUG, logString.c_str());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("has log",
        true, repository_->hasLog());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("log",
        logString, repository_->getLog());
}


void AceLogManagerTest::testCallbackWithEventHandler()
{
    class DummyLogEventHandler : public nsrpc::LogEventHandler
    {
    public:
        virtual void onLog(const char* log) {
            log_ = log;
        }

        const std::string& getLog() const {
            return log_;
        }
    private:
        std::string log_;
    };

    DummyLogEventHandler logEventHandler;
    boost::scoped_ptr<LogRepository> repository(
        LogRepositoryFactory::create(false, &logEventHandler));

    nsrpc::LogManager::unsilent();
    nsrpc::LogManager::redirectToCallback(*repository);

    const std::string logString("Hello, World!!");
    int aceError = ACE_Log_Msg::last_error_adapter();
    ACE_Log_Msg *ace = ACE_Log_Msg::instance();
    ace->conditional_set(__FILE__, __LINE__, 0, aceError);
    ace->log(LM_DEBUG, logString.c_str());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("log",
        logString, logEventHandler.getLog());
}
