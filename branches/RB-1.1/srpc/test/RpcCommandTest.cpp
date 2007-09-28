#include "stdafx.h"
#include "DummyRpcCommand.h"
#include "StreamTexture.h"

using namespace srpc;

/**
* @class RpcCommandTest 
*
* RpcCommand test
*/
class RpcCommandTest : public BitStreamTexture
{
    CPPUNIT_TEST_SUITE(RpcCommandTest );
    CPPUNIT_TEST(testMarshal);
    CPPUNIT_TEST_SUITE_END();
private:
    void testMarshal();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcCommandTest );

void RpcCommandTest::testMarshal()
{
    DummyRpcCommand rpcCommand(100, -100);
    rpcCommand.marshal(*ostream_);

    RpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        rpcCommand.getRpcId(), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        100, p1);
    Int32 p2;
    istream_->read(p2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        -100, p2);
}
