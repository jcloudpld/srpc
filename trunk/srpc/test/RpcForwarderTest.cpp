#include "stdafx.h"
#include "DummyRpcClient.h"
#include "MockRpcNetwork.h"
#include <srpc/detail/ForwardingFunctors.h>
#include <srpc/detail/RpcId.h>
#include <srpc/detail/OBitStream.h>
#include <srpc/StreamFactory.h>

using namespace srpc;

/**
* @class RpcForwarderTest 
*
* RpcForwarder test
*/
class RpcForwarderTest  : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(RpcForwarderTest );
    CPPUNIT_TEST(testHint);
    CPPUNIT_TEST(testParm0);
    CPPUNIT_TEST(testParm1);
    CPPUNIT_TEST(testParm2);
    CPPUNIT_TEST(testParm3);
    CPPUNIT_TEST(testParm4);
    CPPUNIT_TEST(testParm5);
    CPPUNIT_TEST(testParm6);
    CPPUNIT_TEST(testParm7);
    CPPUNIT_TEST(testParmBits);
    CPPUNIT_TEST(testMarshalingError);
    CPPUNIT_TEST(testOnForwarding);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testHint();
    void testParm0();
    void testParm1();
    void testParm2();
    void testParm3();
    void testParm4();
    void testParm5();
    void testParm6();
    void testParm7();
    void testParmBits();
    void testMarshalingError();
    void testOnForwarding();
private:
    MockRpcNetwork* rpcNetwork_;
    DummyRpc* request_;
    srpc::IStream* istream_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcForwarderTest );

void RpcForwarderTest::setUp()
{
    rpcNetwork_ = new MockRpcNetwork;
    request_ = new DummyRpcClient(rpcNetwork_);
    istream_ = StreamFactory::createIStream(StreamFactory::stBit,
        rpcNetwork_->getStreamBuffer());
}


void RpcForwarderTest::tearDown()
{
    delete request_;
    delete rpcNetwork_;
    delete istream_;
}


void RpcForwarderTest::testHint()
{
    request_->rpc0(reinterpret_cast<const void*>(12345));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        12345, rpcNetwork_->getSentRpcHint());
}


void RpcForwarderTest::testParm0()
{
    request_->rpc0(reinterpret_cast<const void*>(12345));

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc0_0"), id);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        12345, rpcNetwork_->getSentRpcHint());
}


void RpcForwarderTest::testParm1()
{
    request_->rpc1(1);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc1_1"), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
}


void RpcForwarderTest::testParm2()
{
    request_->rpc2(1, 2);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc2_2"), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
    Int32 p2;
    istream_->read(p2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 2, p2);
}


void RpcForwarderTest::testParm3()
{
    request_->rpc3(1, 2, 3);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc3_3"), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
    Int32 p2;
    istream_->read(p2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 2, p2);
    Int32 p3;
    istream_->read(p3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 3, p3);
}


void RpcForwarderTest::testParm4()
{
    request_->rpc4(1, 2, 3, 4);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc4_4"), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
    Int32 p2;
    istream_->read(p2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 2, p2);
    Int32 p3;
    istream_->read(p3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 3, p3);
    Int32 p4;
    istream_->read(p4);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 4, p4);
}


void RpcForwarderTest::testParm5()
{
    request_->rpc5(1, 2, 3, 4, 5);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc5_5"), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
    Int32 p2;
    istream_->read(p2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 2, p2);
    Int32 p3;
    istream_->read(p3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 3, p3);
    Int32 p4;
    istream_->read(p4);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 4, p4);
    Int32 p5;
    istream_->read(p5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 5, p5);
}


void RpcForwarderTest::testParm6()
{
    request_->rpc6(1, 2, 3, 4, 5, 6);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc6_6"), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
    Int32 p2;
    istream_->read(p2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 2, p2);
    Int32 p3;
    istream_->read(p3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 3, p3);
    Int32 p4;
    istream_->read(p4);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 4, p4);
    Int32 p5;
    istream_->read(p5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 5, p5);
    Int32 p6;
    istream_->read(p6);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6", 6, p6);
}


void RpcForwarderTest::testParm7()
{
    request_->rpc7(1, 2, 3, 4, 5, 6, 7);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpc7_7"), id);
    Int32 p1;
    istream_->read(p1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
    Int32 p2;
    istream_->read(p2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 2, p2);
    Int32 p3;
    istream_->read(p3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3", 3, p3);
    Int32 p4;
    istream_->read(p4);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4", 4, p4);
    Int32 p5;
    istream_->read(p5);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5", 5, p5);
    Int32 p6;
    istream_->read(p6);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6", 6, p6);
    Int32 p7;
    istream_->read(p7);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p7", 7, p7);
}


void RpcForwarderTest::testParmBits()
{
    request_->rpcBits(1, 2);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        RRpcId("DummyRpc_rpcBits_2"), id);
    Int32 p1;
    istream_->read(p1, 15);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1", 1, p1);
    Int32 p2;
    istream_->read(p2, 31);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2", 2, p2);

    srpc::OStream& ostream = rpcNetwork_->getOutputStream();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("holding bits",
        0, static_cast<OBitStream&>(ostream).getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bits",
        toBytes(32 + 15 + 31) * CHAR_BIT, ostream.getTotalBitCount());
}


void RpcForwarderTest::testMarshalingError()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("no error",
        false, rpcNetwork_->sendFailed());

    rpcNetwork_->setMarshalingError();
    request_->rpc0();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("error",
        true, rpcNetwork_->sendFailed());
}


void RpcForwarderTest::testOnForwarding()
{
    request_->rpc6(1, 2, 3, 4, 5, 6);

    DummyRpcClient* client = static_cast<DummyRpcClient*>(request_);

    RRpcId id;
    id.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("id",
        id.get(),
        client->getLastRpcId().get());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("RPC method name",
        std::string("rpc6"),
        std::string(client->getLastRpcId().getMethodName()));
}
