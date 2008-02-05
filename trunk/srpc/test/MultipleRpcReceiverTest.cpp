#include "stdafx.h"
#include "DummyRpcServer.h"
#include "MockRpcNetwork.h"
#include "StreamTexture.h"
#include <srpc/detail/ForwardingFunctors.h>
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class MultipleRpcReceiverTest 
*
* RpcReceiver test
*/
class MultipleRpcReceiverTest : public BitStreamTexture
{
    CPPUNIT_TEST_SUITE(MultipleRpcReceiverTest );
    CPPUNIT_TEST(testDummyRpcParm1);
    CPPUNIT_TEST(testDummyRpc2Parm1);
    CPPUNIT_TEST(testUnknownRpcMethod);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testDummyRpcParm1();
    void testDummyRpc2Parm1();
    void testUnknownRpcMethod();
private:
    MockRpcNetwork* rpcNetwork_;
    DummyRpcServer* response_;
    DummyRpcServer2* response2_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MultipleRpcReceiverTest );

void MultipleRpcReceiverTest::setUp()
{
    BitStreamTexture::setUp();

    rpcNetwork_ = new MockRpcNetwork;
    response_ = new DummyRpcServer(rpcNetwork_);
    response2_ = new DummyRpcServer2(rpcNetwork_);
}


void MultipleRpcReceiverTest::tearDown()
{
    delete rpcNetwork_;
    delete response_;
    delete response2_;
}


void MultipleRpcReceiverTest::testDummyRpcParm1()
{
    RRpcId rpcId("DummyRpc_rpc1_1");
    rpcId.write(*ostream_);
    RInt32(-1).write(*ostream_);
    ostream_->align();
    rpcNetwork_->onReceive(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId.get(), response_->getRpcId().get());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        -1, response_->getP1());
}


void MultipleRpcReceiverTest::testDummyRpc2Parm1()
{
    RRpcId rpcId("DummyRpc2_rpc1_1");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    ostream_->align();
    rpcNetwork_->onReceive(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId.get(), response2_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response2_->getP1());
}


void MultipleRpcReceiverTest::testUnknownRpcMethod()
{
    RRpcId rpcId("DummyRpc_rpc1_unknown");
    rpcId.write(*ostream_);
    RInt32(-1).write(*ostream_);
    ostream_->align();
    try
    {
        rpcNetwork_->onReceive(*istream_);
        CPPUNIT_FAIL("unknown rpc method");
    }
    catch (const UnknownRpcMethodException&) {}
}
