#include "stdafx.h"
#include "DummyRpcServer.h"
#include "MockRpcNetwork.h"
#include "StreamTexture.h"
#include <srpc/detail/IBitStream.h>
#include <srpc/detail/ForwardingFunctors.h>
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class RpcReceiverTest 
*
* RpcReceiver test
*/
class RpcReceiverTest : public BitStreamTexture
{
    CPPUNIT_TEST_SUITE(RpcReceiverTest );
    //CPPUNIT_TEST(testParm0);
    //CPPUNIT_TEST(testParm1);
    //CPPUNIT_TEST(testParm2);
    //CPPUNIT_TEST(testParm3);
    //CPPUNIT_TEST(testParm4);
    //CPPUNIT_TEST(testParm5);
    //CPPUNIT_TEST(testParm6);
    //CPPUNIT_TEST(testParm7);
    //CPPUNIT_TEST(testParm8);
    //CPPUNIT_TEST(testParm9);
    //CPPUNIT_TEST(testParm10);
    //CPPUNIT_TEST(testParmBits);
    //CPPUNIT_TEST(testAlign);
    //CPPUNIT_TEST(testUnmarshalingError);
    //CPPUNIT_TEST(testUnknownRpcMethod);
    CPPUNIT_TEST(testRpcFailed);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testParm0();
    void testParm1();
    void testParm2();
    void testParm3();
    void testParm4();
    void testParm5();
    void testParm6();
    void testParm7();
    void testParm8();
    void testParm9();
    void testParm10();
    void testParmBits();
    void testAlign();
    void testUnmarshalingError();
    void testUnknownRpcMethod();
    void testRpcFailed();
private:
    MockRpcNetwork* rpcNetwork_;
    DummyRpcServer* response_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcReceiverTest );

void RpcReceiverTest::setUp()
{
    BitStreamTexture::setUp();

    rpcNetwork_ = new MockRpcNetwork;
    response_ = new DummyRpcServer(rpcNetwork_);
}


void RpcReceiverTest::tearDown()
{
    delete rpcNetwork_;
    delete response_;
}


void RpcReceiverTest::testParm0()
{
    RpcId rpcId("DummyRpc_rpc0_0");
    rpcId.write(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        0, response_->getHint());
}


void RpcReceiverTest::testParm1()
{
    RpcId rpcId("DummyRpc_rpc1_1");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(1));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        1, response_->getHint());
}


void RpcReceiverTest::testParm2()
{
    RpcId rpcId("DummyRpc_rpc2_2");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        2, response_->getHint());
}


void RpcReceiverTest::testParm3()
{
    RpcId rpcId("DummyRpc_rpc3_3");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(3));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        3, response_->getHint());
}


void RpcReceiverTest::testParm4()
{
    RpcId rpcId("DummyRpc_rpc4_4");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(4));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        4, response_->getP4());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        4, response_->getHint());
}


void RpcReceiverTest::testParm5()
{
    RpcId rpcId("DummyRpc_rpc5_5");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(5));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        4, response_->getP4());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5",
        5, response_->getP5());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        5, response_->getHint());
}


void RpcReceiverTest::testParm6()
{
    RpcId rpcId("DummyRpc_rpc6_6");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);
    RInt32(6).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(6));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        4, response_->getP4());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5",
        5, response_->getP5());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6",
        6, response_->getP6());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        6, response_->getHint());
}


void RpcReceiverTest::testParm7()
{
    RpcId rpcId("DummyRpc_rpc7_7");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);
    RInt32(6).write(*ostream_);
    RInt32(7).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(7));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        4, response_->getP4());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5",
        5, response_->getP5());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6",
        6, response_->getP6());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p7",
        7, response_->getP7());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        7, response_->getHint());
}


void RpcReceiverTest::testParm8()
{
    RpcId rpcId("DummyRpc_rpc8_8");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);
    RInt32(6).write(*ostream_);
    RInt32(7).write(*ostream_);
    RInt32(8).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(8));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        4, response_->getP4());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5",
        5, response_->getP5());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6",
        6, response_->getP6());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p7",
        7, response_->getP7());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p8",
        8, response_->getP8());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        8, response_->getHint());
}


void RpcReceiverTest::testParm9()
{
    RpcId rpcId("DummyRpc_rpc9_9");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);
    RInt32(6).write(*ostream_);
    RInt32(7).write(*ostream_);
    RInt32(8).write(*ostream_);
    RInt32(9).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(9));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        4, response_->getP4());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5",
        5, response_->getP5());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6",
        6, response_->getP6());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p7",
        7, response_->getP7());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p8",
        8, response_->getP8());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p9",
        9, response_->getP9());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        9, response_->getHint());
}


void RpcReceiverTest::testParm10()
{
    RpcId rpcId("DummyRpc_rpc10_10");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(-2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(-4).write(*ostream_);
    RInt32(5).write(*ostream_);
    RInt32(-6).write(*ostream_);
    RInt32(7).write(*ostream_);
    RInt32(-8).write(*ostream_);
    RInt32(9).write(*ostream_);
    RInt32(-10).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(10));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        -2, response_->getP2());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p3",
        3, response_->getP3());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p4",
        -4, response_->getP4());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p5",
        5, response_->getP5());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p6",
        -6, response_->getP6());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p7",
        7, response_->getP7());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p8",
        -8, response_->getP8());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p9",
        9, response_->getP9());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p10",
        -10, response_->getP10());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("hint",
        10, response_->getHint());
}


void RpcReceiverTest::testParmBits()
{
    RpcId rpcId("DummyRpc_rpcBits");
    rpcId.write(*ostream_);
    RInt15(1).write(*ostream_);
    RInt31(2).write(*ostream_);

    rpcNetwork_->onReceive(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("rpc id",
        rpcId, response_->getRpcId());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p1",
        1, response_->getP1());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("p2",
        2, response_->getP2());
}


void RpcReceiverTest::testAlign()
{
    RpcId rpcId("DummyRpc_rpcBits_2");

    for (int i = 0; i < 3; ++i) {
        rpcId.write(*ostream_);
        RInt15(1).write(*ostream_);
        RInt31(-1).write(*ostream_);
        ostream_->align();
    }

    for (int i = 0; i < 3; ++i) {
        rpcNetwork_->onReceive(*istream_);
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%d - rpcId") % i).str(),
            rpcId, response_->getRpcId());
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%d - p1") % i).str(),
            1, response_->getP1());
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%d - p2") % i).str(),
            -1, response_->getP2());
        CPPUNIT_ASSERT_EQUAL_MESSAGE(
            (boost::format("#%d - read bit count") % i).str(),
            0, static_cast<IBitStream&>(*istream_).getReadBitCount());
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
}


void RpcReceiverTest::testUnmarshalingError()
{
    try {
        rpcNetwork_->onReceive(*istream_);
        CPPUNIT_FAIL("streaming error");
    }
    catch (const StreamingException&) {}
}


void RpcReceiverTest::testUnknownRpcMethod()
{
    RpcId rpcId("DummyRpc_rpc0_unknown");
    rpcId.write(*ostream_);

    try {
        rpcNetwork_->onReceive(*istream_);
        CPPUNIT_FAIL("unknown rpc rpcId");
    }
    catch (const UnknownRpcMethodException&) {}
}


void RpcReceiverTest::testRpcFailed()
{
    RpcId rpcId("DummyRpc_rpcFailed_0");
    rpcId.write(*ostream_);

    try {
        rpcNetwork_->onReceive(*istream_);
        CPPUNIT_FAIL("unknown rpc rpcId");
    }
    catch (const RpcFailedException&) {}
}
