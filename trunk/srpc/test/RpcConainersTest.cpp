#include "stdafx.h"
#include <srpc/RpcContainers.h>
#include "StreamTexture.h"

using namespace srpc;

/**
* @class RpcConainersTest 
*
* Rpc containers test
*/
class RpcConainersTest : public BitStreamTexture
{
};


TEST_F(RpcConainersTest, testRVector)
{
    RVector<RInt32> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.write(*ostream_);

    RVector<RInt32> actual;
    actual.read(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ(expected.back(), actual.back());
}


TEST_F(RpcConainersTest, testRList)
{
    RList<RInt32> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.write(*ostream_);

    RList<RInt32> actual;
    actual.read(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ(expected.back(), actual.back());
}


TEST_F(RpcConainersTest, testRSet)
{
    RSet<RShortString> expected;
    expected.insert("1234");
    expected.insert("abcd");
    expected.insert("4567");
    expected.write(*ostream_);

    RSet<RShortString> actual;
    actual.read(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ(*expected.begin(), *actual.begin());
}


TEST_F(RpcConainersTest, testRMap)
{
    typedef RMap<RInt32, RShortString> MapType;
    MapType expected;
    expected.insert(MapType::value_type(3, "1234"));
    expected.insert(MapType::value_type(1, "1234"));
    expected.insert(MapType::value_type(2, "1234"));
    expected.write(*ostream_);

    MapType actual;
    actual.read(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ((*expected.begin()).second, (*actual.begin()).second);
}
