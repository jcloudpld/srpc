#ifndef NSRPC_ACETESTFIXTURE_H
#define NSRPC_ACETESTFIXTURE_H

#include <nsrpc/utility/LogManager.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702 4312 4512)
#endif
#include <cppunit/TestFixture.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

//#define PRINT_ACE_LOG

/**
 * @class AceTestFixture
 *
 * ACE 초기화를 위한 TestFixture
 */
class AceTestFixture : public testing::Test
{
protected:
    virtual void SetUp() {
#ifdef USE_CPPUNIT_PLUGIN_TESTER
        ACE::init();
#endif

#ifdef PRINT_ACE_LOG
        ACE::debug(1);
        nsrpc::LogManager::verbose();
        //logManager_.redirectToFile("nsrpcTest.log");
#else
        nsrpc::LogManager::silent();
#endif
    }

    virtual void TearDown() {
#ifdef USE_CPPUNIT_PLUGIN_TESTER
        ACE::fini();
#endif
    }
private:
    //nsrpc::LogManager logManager_;
};

#endif // !defined(NSRPC_ACETESTFIXTURE_H)
