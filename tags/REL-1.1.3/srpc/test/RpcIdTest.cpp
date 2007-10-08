#include "stdafx.h"
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class RpcIdTest 
* RpcId test
*/
class RpcIdTest  : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(RpcIdTest );
    CPPUNIT_TEST(testRpcId);
    CPPUNIT_TEST_SUITE_END();
private:
    void testRpcId();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcIdTest );


void RpcIdTest::testRpcId()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("same",
        RpcId("abc"), RpcId("abc"));
    CPPUNIT_ASSERT_MESSAGE("not same",
        RpcId("a") != RpcId("b"));
}
