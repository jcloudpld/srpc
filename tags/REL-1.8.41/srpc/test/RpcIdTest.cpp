#include "stdafx.h"
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class RpcIdTest 
* RRpcId test
*/
class RpcIdTest : public testing::Test
{
};


TEST_F(RpcIdTest, testRpcId)
{
    EXPECT_EQ(RRpcId("abc"), RRpcId("abc"));
    EXPECT_NE(RRpcId("a"), RRpcId("b"));
}
