#include "stdafx.h"
#include "DummyRpcCommand.h"
#include "StreamFixture.h"

using namespace srpc;

/**
* @class RpcCommandTest 
*
* RpcCommand test
*/
class RpcCommandTest : public BitStreamFixture
{
};


TEST_F(RpcCommandTest, testMarshal)
{
    DummyRpcCommand rpcCommand(100, -100);
    rpcCommand.marshal(*ostream_);

    RRpcId id;
    id.serialize(*istream_);
    EXPECT_EQ(rpcCommand.getRpcId().get(), id.get());
    Int32 p1;
    istream_->read(p1);
    EXPECT_EQ(100, p1);
    Int32 p2;
    istream_->read(p2);
    EXPECT_EQ(-100, p2);
}
