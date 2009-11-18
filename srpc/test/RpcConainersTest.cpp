#include "stdafx.h"
#include <srpc/RpcContainers.h>
#include "StreamFixture.h"

using namespace srpc;

/**
* @class RpcConainersTest 
*
* Rpc containers test
*/
class RpcConainersTest : public BitStreamFixture
{
};


TEST_F(RpcConainersTest, testRVector)
{
    RVector<Int32> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.serialize(*ostream_);

    RVector<Int32> actual;
    actual.serialize(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ(expected.back(), actual.back());
}


TEST_F(RpcConainersTest, testRList)
{
    RList<Int32> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.serialize(*ostream_);

    RList<Int32> actual;
    actual.serialize(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ(expected.back(), actual.back());
}


TEST_F(RpcConainersTest, testRSet)
{
    RSet<RShortString> expected;
    expected.insert("1234");
    expected.insert("abcd");
    expected.insert("4567");
    expected.serialize(*ostream_);

    RSet<RShortString> actual;
    actual.serialize(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ(*expected.begin(), *actual.begin());
}


TEST_F(RpcConainersTest, testRMap)
{
    typedef RMap<Int32, RShortString> MapType;
    MapType expected;
    expected.insert(MapType::value_type(3, "1234"));
    expected.insert(MapType::value_type(1, "1234"));
    expected.insert(MapType::value_type(2, "1234"));
    expected.serialize(*ostream_);

    MapType actual;
    actual.serialize(*istream_);

    EXPECT_EQ(expected.size(), actual.size());

    EXPECT_EQ((*expected.begin()).second, (*actual.begin()).second);
}
