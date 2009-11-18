#include "stdafx.h"
#include <srpc/RpcTypes.h>
#include <srpc/RpcContainers.h>
#include <srpc/utility/Unicode.h>
#include "StreamFixture.h"
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class RpcTypesTest 
*
* Rpc Types test
* - I(Bit|Byte)StreamTest와 어느 정도 유사하지만 RpcType 테스트를 한 눈에
*   볼 수 있도록 의도적으로 중복 시킴
*/
class RpcTypesTest : public BitStreamFixture
{
};


TEST_F(RpcTypesTest, testRInt8)
{
    RInt8 expected = SCHAR_MIN;
    expected.serialize(*ostream_);

    RInt8 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(static_cast<int>(expected), static_cast<int>(actual));
}


TEST_F(RpcTypesTest, testRUInt8)
{
    RUInt8 expected = SCHAR_MAX;
    expected.serialize(*ostream_);

    RUInt8 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt16)
{
    RInt16 expected = SHRT_MIN;
    expected.serialize(*ostream_);

    RInt16 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt16)
{
    RUInt16 expected = SHRT_MAX;
    expected.serialize(*ostream_);

    RUInt16 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt32)
{
    RInt32 expected = INT_MIN;
    expected.serialize(*ostream_);

    RInt32 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt32)
{
    RUInt32 expected = INT_MAX;
    expected.serialize(*ostream_);

    RUInt32 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt64)
{
    RInt64 expected = -1;
    expected.serialize(*ostream_);

    RInt64 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt64)
{
    RUInt64 expected = static_cast<UInt64>(-1);
    expected.serialize(*ostream_);

    RUInt64 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRFloat32)
{
    RFloat32 expected = -123.456F;
    expected.serialize(*ostream_);

    RFloat32 actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRString)
{
    RString expected = "TANSTAAFL";
    expected.serialize(*ostream_);

    RString actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRShortString)
{
    RShortString expected(String(256, 'X'));
    expected.serialize(*ostream_);

    RShortString actual;
    actual.serialize(*istream_);
    EXPECT_EQ(static_cast<int>(expected.size() - 1),
        static_cast<int>(actual.size()));
}


TEST_F(RpcTypesTest, testRBool)
{
    {
        RBool expected = false;
        expected.serialize(*ostream_);

        RBool actual;
        actual.serialize(*istream_);
        EXPECT_EQ(expected, actual);
    }
    {
        RBool expected = true;
        expected.serialize(*ostream_);

        RBool actual;
        actual.serialize(*istream_);
        EXPECT_EQ(expected, actual);
    }
}


TEST_F(RpcTypesTest, testRRpcId)
{
    RRpcId expected("rpc_id");
    expected.serialize(*ostream_);

    RRpcId actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected.get(), actual.get());
}


TEST_F(RpcTypesTest, testRWString)
{
    RWString expected(fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.serialize(*ostream_);

    RWString actual;
    actual.serialize(*istream_);
    EXPECT_TRUE(wcscmp(expected.c_str(), actual.c_str()) == 0);
}


TEST_F(RpcTypesTest, testRWShortString)
{
    RWShortString expected(fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.serialize(*ostream_);

    RWShortString actual;
    actual.serialize(*istream_);
    EXPECT_TRUE(wcscmp(expected.c_str(), actual.c_str()) == 0);
}


TEST_F(RpcTypesTest, testRVector)
{
    RVector<Int32, 8> expected;
    for (int i = 0; i < 10; ++i) {
        expected.push_back(i);
    }
    expected.serialize(*ostream_);

    RVector<Int32, 8> actual;
    actual.serialize(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}


TEST_F(RpcTypesTest, testRList)
{
    RList<Int32, 7> expected;
    for (int i = 0; i < 10; ++i) {
        expected.push_back(i);
    }
    expected.serialize(*ostream_);

    RList<Int32, 7> actual;
    actual.serialize(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}


TEST_F(RpcTypesTest, testRSet)
{
    RSet<Int32> expected;
    for (int i = 0; i < 10; ++i) {
        expected.insert(i);
    }
    expected.serialize(*ostream_);

    RSet<Int32> actual;
    actual.serialize(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}


enum EnumType { etA, etB, etC };
typedef RpcIntType<EnumType, 3> REnumType;

TEST_F(RpcTypesTest, testEnum)
{
    EnumType exp(etC);
    REnumType expected(exp);
    expected.serialize(*ostream_);

    REnumType actual;
    actual.serialize(*istream_);
    EnumType act = actual;
    EXPECT_EQ(expected, actual);
    EXPECT_EQ(exp, act);
}


TEST_F(RpcTypesTest, testRUserDefinedString)
{
    typedef RpcStringType<String, 10> RUserDefinedString;

    RUserDefinedString expected(String(12, 'X'));
    expected.serialize(*ostream_);

    RUserDefinedString actual;
    actual.serialize(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size() - 2),
        static_cast<int>(actual.size()));
}


TEST_F(RpcTypesTest, testRMap)
{
    RMap<Int32, RShortString> expected;
    for (int i = 0; i < 10; ++i) {
        OStringStream oss;
        oss << "#" << i;
        expected.insert(std::make_pair(i, oss.str()));
    }
    expected.serialize(*ostream_);

    RMap<Int32, RShortString> actual;
    actual.serialize(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}


TEST_F(RpcTypesTest, testRStringMaxLength)
{
    const size_t maxLength = 9;
    typedef RpcStringType<String, maxLength> RLimitedString;

    RLimitedString expected = "1234567890";
    expected.serialize(*ostream_);

    RLimitedString actual;
    actual.serialize(*istream_);
    EXPECT_EQ(expected.substr(0, maxLength), actual.ref());
}
