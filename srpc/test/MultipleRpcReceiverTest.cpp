#include "stdafx.h"
#include "DummyRpcServer.h"
#include "MockRpcNetwork.h"
#include "StreamFixture.h"
#include <srpc/detail/ForwardingFunctors.h>
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class MultipleRpcReceiverTest 
*
* RpcReceiver test
*/
class MultipleRpcReceiverTest : public BitStreamFixture
{
private:
    virtual void SetUp() {
        BitStreamFixture::SetUp();

        rpcNetwork_ = new MockRpcNetwork;
        response_ = new DummyRpcServer(rpcNetwork_);
        response2_ = new DummyRpcServer2(rpcNetwork_);
    }

    virtual void tearDown() {
        delete rpcNetwork_;
        delete response_;
        delete response2_;
    }

protected:
    MockRpcNetwork* rpcNetwork_;
    DummyRpcServer* response_;
    DummyRpcServer2* response2_;
};


TEST_F(MultipleRpcReceiverTest, testDummyRpcParm1)
{
    RRpcId rpcId("DummyRpc_rpc1_1");
    rpcId.serialize(*ostream_);
    RInt32(-1).serialize(*ostream_);
    ostream_->align();
    rpcNetwork_->onReceive(*istream_);
    EXPECT_EQ(rpcId.get(), response_->getRpcId().get());
    EXPECT_EQ(-1, response_->getP1());
}


TEST_F(MultipleRpcReceiverTest, testDummyRpc2Parm1)
{
    RRpcId rpcId("DummyRpc2_rpc1_1");
    rpcId.serialize(*ostream_);
    RInt32(1).serialize(*ostream_);
    ostream_->align();
    rpcNetwork_->onReceive(*istream_);
    EXPECT_EQ(rpcId.get(), response2_->getRpcId());
    EXPECT_EQ(1, response2_->getP1());
}


TEST_F(MultipleRpcReceiverTest, testUnknownRpcMethod)
{
    RRpcId rpcId("DummyRpc_rpc1_unknown");
    rpcId.serialize(*ostream_);
    RInt32(-1).serialize(*ostream_);
    ostream_->align();

    EXPECT_THROW(
        rpcNetwork_->onReceive(*istream_), UnknownRpcMethodException);
}
