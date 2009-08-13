#include "stdafx.h"
#include "DummyRpcServer.h"
#include "MockRpcNetwork.h"
#include "StreamFixture.h"
#include <srpc/detail/IBitStream.h>
#include <srpc/detail/ForwardingFunctors.h>
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class RpcReceiverTest 
*
* RpcReceiver test
*/
class RpcReceiverTest : public BitStreamFixture
{
private:
    virtual void SetUp() {
        BitStreamFixture::SetUp();

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
    rpcId.serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(0, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm1)
{
    RRpcId rpcId("DummyRpc_rpc1_1");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(1));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(1, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm2)
{
    RRpcId rpcId("DummyRpc_rpc2_2");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, reinterpret_cast<const void*>(2));
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
    EXPECT_EQ(2, response_->getHint());
}


TEST_F(RpcReceiverTest, testParm3)
{
    RRpcId rpcId("DummyRpc_rpc3_3");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);

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
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);

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
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);
    RInt32(5).serialize(*ostream_);

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
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);
    RInt32(5).serialize(*ostream_);
    RInt32(6).serialize(*ostream_);

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
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    RInt32(2).serialize(*ostream_);
    RInt32(3).serialize(*ostream_);
    RInt32(4).serialize(*ostream_);
    RInt32(5).serialize(*ostream_);
    RInt32(6).serialize(*ostream_);
    RInt32(7).serialize(*ostream_);

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
    rpcId.serialize(*ostream_);
    RInt15(1).serialize(*ostream_);
    RInt31(2).serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_);
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(1, response_->getP1());
    EXPECT_EQ(2, response_->getP2());
}


TEST_F(RpcReceiverTest, testAlign)
{
    RRpcId rpcId("DummyRpc_rpcBits_2");

    for (int i = 0; i < 3; ++i) {
        rpcId.serialize(*ostream_);
        RInt15(1).serialize(*ostream_);
        RInt31(-1).serialize(*ostream_);
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
    rpcId.serialize(*ostream_);

    EXPECT_THROW(rpcNetwork_->onReceive(*istream_), UnknownRpcMethodException);
}


TEST_F(RpcReceiverTest, testRpcFailed)
{
    RRpcId rpcId("DummyRpc_rpcFailed_0");
    rpcId.serialize(*ostream_);

    EXPECT_THROW(rpcNetwork_->onReceive(*istream_), RpcFailedException);
}


TEST_F(RpcReceiverTest, testRpcPrototype)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    EXPECT_EQ(std::string("rpc0"),
        std::string(response_->getRpcId().getMethodName()));
}


TEST_F(RpcReceiverTest, testOnReceiving)
{
    RRpcId rpcId("DummyRpc_rpc0_0");
    rpcId.serialize(*ostream_);

    rpcNetwork_->onReceive(*istream_, 0);
    EXPECT_EQ(std::string("rpc0"),
        std::string(response_->getLastRpcId().getMethodName()));
}
