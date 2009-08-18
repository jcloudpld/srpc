#include "stdafx.h"
#include "DummyRpcEvent.h"
#include "StreamFixture.h"

using namespace srpc;

/**
* @class RpcEventTest 
*
* RpcEvent test
*/
class RpcEventTest : public BitStreamFixture
{
};


TEST_F(RpcEventTest, testCalling)
{
    RInt32(100).serialize(*ostream_);
    RInt32(-100).serialize(*ostream_);

    DummyRpcEvent event;
    event.dispatch(&event, *istream_);

    EXPECT_EQ(100, event.getP1());
    EXPECT_EQ(-100, event.getP2());
}
