#include "stdafx.h"
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class RpcIdTest 
* RRpcId test
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
        RRpcId("abc"), RRpcId("abc"));
    CPPUNIT_ASSERT_MESSAGE("not same",
        RRpcId("a") != RRpcId("b"));
}
