#include "stdafx.h"
#include "DummyRpcEvent.h"
#include "StreamTexture.h"

using namespace srpc;

/**
* @class RpcEventTest 
*
* RpcEvent test
*/
class RpcEventTest : public BitStreamTexture
{
    CPPUNIT_TEST_SUITE(RpcEventTest );
    CPPUNIT_TEST(testCalling);
    CPPUNIT_TEST_SUITE_END();
private:
    void testCalling();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcEventTest );

void RpcEventTest::testCalling()
{
    RInt32(100).write(*ostream_);
    RInt32(-100).write(*ostream_);

    DummyRpcEvent event;
    event.dispatch(event, *istream_);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        100, event.getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        -100, event.getP2());
}
