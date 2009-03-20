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
private:
    virtual void SetUp() {
        BitStreamTexture::SetUp();

        rpcNetwork_ = new MockRpcNetwork;
        response_ = new DummyRpcServer(rpcNetwork_);
    }

    virtual void TearDown() {
        delete rpcNetwork_;
        delete response_;
    }

protected:
    MockRpcNetwork* rpcNetwork_;
    DummyRpcServer* response_;
};


TEST_F(RpcReceiverTest, testParm0)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.write(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(0, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm1)
{
    RRpcId rpcId("DummyRpc_rpc1_1");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(1));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(1, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm2)
{
    RRpcId rpcId("DummyRpc_rpc2_2");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(2));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
    EXPECT_EQ(2, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm3)
{
    RRpcId rpcId("DummyRpc_rpc3_3");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(3));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
    EXPECT_EQ(3, response_->getP3());
    EXPECT_EQ(3, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm4)
{
    RRpcId rpcId("DummyRpc_rpc4_4");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(4));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
    EXPECT_EQ(3, response_->getP3());
    EXPECT_EQ(4, response_->getP4());
    EXPECT_EQ(4, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm5)
{
    RRpcId rpcId("DummyRpc_rpc5_5");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(5));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
    EXPECT_EQ(3, response_->getP3());
    EXPECT_EQ(4, response_->getP4());
    EXPECT_EQ(5, response_->getP5());
    EXPECT_EQ(5, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm6)
{
    RRpcId rpcId("DummyRpc_rpc6_6");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);
    RInt32(6).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(6));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
    EXPECT_EQ(3, response_->getP3());
    EXPECT_EQ(4, response_->getP4());
    EXPECT_EQ(5, response_->getP5());
    EXPECT_EQ(6, response_->getP6());
    EXPECT_EQ(6, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm7)
{
    RRpcId rpcId("DummyRpc_rpc7_7");
    rpcId.write(*ostream_);
    RInt32(1).write(*ostream_);
    RInt32(2).write(*ostream_);
    RInt32(3).write(*ostream_);
    RInt32(4).write(*ostream_);
    RInt32(5).write(*ostream_);
    RInt32(6).write(*ostream_);
    RInt32(7).write(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(7));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
    EXPECT_EQ(3, response_->getP3());
    EXPECT_EQ(4, response_->getP4());
    EXPECT_EQ(5, response_->getP5());
    EXPECT_EQ(6, response_->getP6());
    EXPECT_EQ(7, response_->getP7());
    EXPECT_EQ(7, response_->getHint());
}


TEST_F(RpcReceiverTest, testParmBits)
{
    RRpcId rpcId("DummyRpc_rpcBits_2");
    rpcId.write(*ostream_);
    RInt15(1).write(*ostream_);
    RInt31(2).write(*ostream_);

    rpcNetwork_->onReceive(*istream_);
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
}


TEST_F(RpcReceiverTest, testAlign)
{
    RRpcId rpcId("DummyRpc_rpcBits_2");

    for (int i = 0; i < 3; ++i) {
        rpcId.write(*ostream_);
        RInt15(1).write(*ostream_);
        RInt31(-1).write(*ostream_);
        ostream_->align();
    }

    for (int i = 0; i < 3; ++i) {
        rpcNetwork_->onReceive(*istream_);
        EXPECT_EQ(rpcId.get(), response_->getRpcId().get()) <<
            "#" << i << " - rpcId";
        EXPECT_EQ(1, response_->getP1()) << "#" << i << " - p1";
        EXPECT_EQ(-1, response_->getP2()) << "#" << i << " - p2";;
        EXPECT_EQ(0, static_cast<IBitStream&>(*istream_).getReadBitCount()) <<
            "#" << i << " - read bit count";
    }
    EXPECT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(RpcReceiverTest, testUnmarshalingError)
{
    EXPECT_THROW(rpcNetwork_->onReceive(*istream_), StreamingException);
}


TEST_F(RpcReceiverTest, testUnknownRpcMethod)
{
    RRpcId rpcId("DummyRpc_rpc0_unknown");
    rpcId.write(*ostream_);

    EXPECT_THROW(rpcNetwork_->onReceive(*istream_), UnknownRpcMethodException);
}


TEST_F(RpcReceiverTest, testRpcFailed)
{
    RRpcId rpcId("DummyRpc_rpcFailed_0");
    rpcId.write(*ostream_);

    EXPECT_THROW(rpcNetwork_->onReceive(*istream_), RpcFailedException);
}


TEST_F(RpcReceiverTest, testRpcPrototype)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.write(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    EXPECT_EQ("rpc0", response_->getRpcId().getMethodName());
}


TEST_F(RpcReceiverTest, testOnReceiving)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.write(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    EXPECT_EQ("rpc0", response_->getLastRpcId().getMethodName());
}
