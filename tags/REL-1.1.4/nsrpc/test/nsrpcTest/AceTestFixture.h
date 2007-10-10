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

/**
 * @class AceTestFixture
 *
 * ACE 초기화를 위한 TestFixture
 */
class AceTestFixture : public CppUnit::TestFixture
{
public:
    virtual void setUp() {
#ifdef USE_CPPUNIT_PLUGIN_TESTER
        ACE::init();
#endif

        //ACE::debug(1);
        //nsrpc::LogManager::verbose();
        nsrpc::LogManager::silent();
        //logManager_.redirectToFile("nsrpcTest.log");
    }

    virtual void tearDown() {
#ifdef USE_CPPUNIT_PLUGIN_TESTER
        ACE::fini();
#endif
    }
private:
    //nsrpc::LogManager logManager_;
};

#endif // !defined(NSRPC_ACETESTFIXTURE_H)
