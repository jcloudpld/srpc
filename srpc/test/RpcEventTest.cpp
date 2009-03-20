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
};


TEST_F(RpcEventTest, testCalling)
{
    RInt32(100).write(*ostream_);
    RInt32(-100).write(*ostream_);

    DummyRpcEvent event;
    event.dispatch(&event, *istream_);

    EXPECT_EQ(100, event.getP1());
    EXPECT_EQ(-100, event.getP2());
}
