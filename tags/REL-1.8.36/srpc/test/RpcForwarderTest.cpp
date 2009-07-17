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
class RpcForwarderTest  : public testing::Test
{
private:
    virtual void SetUp() {
        rpcNetwork_ = new MockRpcNetwork;
        request_ = new DummyRpcClient(rpcNetwork_);
        istream_ = StreamFactory::createIStream(StreamFactory::stBit,
            rpcNetwork_->getStreamBuffer());
    }

    virtual void TearDown() {
        delete request_;
        delete rpcNetwork_;
        delete istream_;
    }

protected:
    MockRpcNetwork* rpcNetwork_;
    DummyRpc* request_;
    srpc::IStream* istream_;
};


TEST_F(RpcForwarderTest, testHint)
{
    request_->rpc0(reinterpret_cast<const void*>(12345));

    EXPECT_EQ(12345, rpcNetwork_->getSentRpcHint());
}


TEST_F(RpcForwarderTest, testParm0)
{
    request_->rpc0(reinterpret_cast<const void*>(12345));

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc0_0"), id);
    EXPECT_EQ(12345, rpcNetwork_->getSentRpcHint());
}


TEST_F(RpcForwarderTest, testParm1)
{
    request_->rpc1(1);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc1_1"), id);
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(1, p1);
}


TEST_F(RpcForwarderTest, testParm2)
{
    request_->rpc2(1, 2);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc2_2"), id);
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(1, p1);
    Int32 p2;
    istream_->read(p2);
    EXPECT_EQ(2, p2);
}


TEST_F(RpcForwarderTest, testParm3)
{
    request_->rpc3(1, 2, 3);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc3_3"), id);
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(1, p1);
    Int32 p2;
    istream_->read(p2);
    EXPECT_EQ(2, p2);
    Int32 p3;
    istream_->read(p3);
    EXPECT_EQ(3, p3);
}


TEST_F(RpcForwarderTest, testParm4)
{
    request_->rpc4(1, 2, 3, 4);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc4_4"), id);
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(1, p1);
    Int32 p2;
    istream_->read(p2);
    EXPECT_EQ(2, p2);
    Int32 p3;
    istream_->read(p3);
    EXPECT_EQ(3, p3);
    Int32 p4;
    istream_->read(p4);
    EXPECT_EQ(4, p4);
}


TEST_F(RpcForwarderTest, testParm5)
{
    request_->rpc5(1, 2, 3, 4, 5);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc5_5"), id);
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(1, p1);
    Int32 p2;
    istream_->read(p2);
    EXPECT_EQ(2, p2);
    Int32 p3;
    istream_->read(p3);
    EXPECT_EQ(3, p3);
    Int32 p4;
    istream_->read(p4);
    EXPECT_EQ(4, p4);
    Int32 p5;
    istream_->read(p5);
    EXPECT_EQ(5, p5);
}


TEST_F(RpcForwarderTest, testParm6)
{
    request_->rpc6(1, 2, 3, 4, 5, 6);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc6_6"), id);
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(1, p1);
    Int32 p2;
    istream_->read(p2);
    EXPECT_EQ(2, p2);
    Int32 p3;
    istream_->read(p3);
    EXPECT_EQ(3, p3);
    Int32 p4;
    istream_->read(p4);
    EXPECT_EQ(4, p4);
    Int32 p5;
    istream_->read(p5);
    EXPECT_EQ(5, p5);
    Int32 p6;
    istream_->read(p6);
    EXPECT_EQ(6, p6);
}


TEST_F(RpcForwarderTest, testParm7)
{
    request_->rpc7(1, 2, 3, 4, 5, 6, 7);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpc7_7"), id);
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(1, p1);
    Int32 p2;
    istream_->read(p2);
    EXPECT_EQ(2, p2);
    Int32 p3;
    istream_->read(p3);
    EXPECT_EQ(3, p3);
    Int32 p4;
    istream_->read(p4);
    EXPECT_EQ(4, p4);
    Int32 p5;
    istream_->read(p5);
    EXPECT_EQ(5, p5);
    Int32 p6;
    istream_->read(p6);
    EXPECT_EQ(6, p6);
    Int32 p7;
    istream_->read(p7);
    EXPECT_EQ(7, p7);
}


TEST_F(RpcForwarderTest, testParmBits)
{
    request_->rpcBits(1, 2);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(RRpcId("DummyRpc_rpcBits_2"), id);
    Int32 p1;
    istream_->read(p1, 15);
    EXPECT_EQ(1, p1);
    Int32 p2;
    istream_->read(p2, 31);
    EXPECT_EQ(2, p2);

    srpc::OStream& ostream = rpcNetwork_->getOutputStream();
    EXPECT_EQ(0, static_cast<OBitStream&>(ostream).getHoldingBitCount());
    EXPECT_EQ(toBytes(32 + 15 + 31) * CHAR_BIT, ostream.getTotalBitCount());
}


TEST_F(RpcForwarderTest, testMarshalingError)
{
    EXPECT_FALSE(rpcNetwork_->sendFailed());

    rpcNetwork_->setMarshalingError();
    request_->rpc0();

    EXPECT_TRUE(rpcNetwork_->sendFailed());
}


TEST_F(RpcForwarderTest, testOnForwarding)
{
    request_->rpc6(1, 2, 3, 4, 5, 6);

    DummyRpcClient* client = static_cast<DummyRpcClient*>(request_);

    RRpcId id;
    id.read(*istream_);
    EXPECT_EQ(id.get(), client->getLastRpcId().get());

    EXPECT_EQ(std::string("rpc6"),
        std::string(client->getLastRpcId().getMethodName()));
}
