#include "stdafx.h"
#include <srpc/RpcTypes.h>
#include <srpc/RpcContainers.h>
#include <srpc/utility/Unicode.h>
#include "StreamTexture.h"
#include <srpc/detail/RpcId.h>

using namespace srpc;

/**
* @class RpcTypesTest 
*
* Rpc Types test
* - I(Bit|Byte)StreamTest와 어느 정도 유사하지만 RpcType 테스트를 한 눈에
*   볼 수 있도록 의도적으로 중복 시킴
*/
class RpcTypesTest : public BitStreamTexture
{
};


TEST_F(RpcTypesTest, testRInt8)
{
    RInt8 expected = SCHAR_MIN;
    expected.write(*ostream_);

    RInt8 actual;
    actual.read(*istream_);
    EXPECT_EQ(static_cast<int>(expected), static_cast<int>(actual));
}


TEST_F(RpcTypesTest, testRUInt8)
{
    RUInt8 expected = SCHAR_MAX;
    expected.write(*ostream_);

    RUInt8 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt16)
{
    RInt16 expected = SHRT_MIN;
    expected.write(*ostream_);

    RInt16 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt16)
{
    RUInt16 expected = SHRT_MAX;
    expected.write(*ostream_);

    RUInt16 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt32)
{
    RInt32 expected = INT_MIN;
    expected.write(*ostream_);

    RInt32 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt32)
{
    RUInt32 expected = INT_MAX;
    expected.write(*ostream_);

    RUInt32 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRInt64)
{
    RInt64 expected = -1;
    expected.write(*ostream_);

    RInt64 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRUInt64)
{
    RUInt64 expected = static_cast<UInt64>(-1);
    expected.write(*ostream_);

    RUInt64 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRFloat32)
{
    RFloat32 expected = -123.456F;
    expected.write(*ostream_);

    RFloat32 actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRString)
{
    RString expected = "독도는 우리땅!";
    expected.write(*ostream_);

    RString actual;
    actual.read(*istream_);
    EXPECT_EQ(expected, actual);
}


TEST_F(RpcTypesTest, testRShortString)
{
    RShortString expected(String(256, 'X'));
    expected.write(*ostream_);

    RShortString actual;
    actual.read(*istream_);
    EXPECT_EQ(static_cast<int>(expected.size() - 1),
        static_cast<int>(actual.size()));
}


TEST_F(RpcTypesTest, testRBool)
{
    {
        RBool expected = false;
        expected.write(*ostream_);

        RBool actual;
        actual.read(*istream_);
        EXPECT_EQ(expected, actual);
    }
    {
        RBool expected = true;
        expected.write(*ostream_);

        RBool actual;
        actual.read(*istream_);
        EXPECT_EQ(expected, actual);
    }
}


TEST_F(RpcTypesTest, testRRpcId)
{
    RRpcId expected("rpc_id");
    expected.write(*ostream_);

    RRpcId actual;
    actual.read(*istream_);
    EXPECT_EQ(expected.get(), actual.get());
}


TEST_F(RpcTypesTest, testRWString)
{
    RWString expected(fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.write(*ostream_);

    RWString actual;
    actual.read(*istream_);
    EXPECT_EQ(expected.ref(), actual.ref());
}


TEST_F(RpcTypesTest, testRWShortString)
{
    RWShortString expected(fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.write(*ostream_);

    RWShortString actual;
    actual.read(*istream_);
    EXPECT_EQ(expected.ref(), actual.ref());
}


TEST_F(RpcTypesTest, testRVector)
{
    RVector<RInt32, 8> expected;
    for (int i = 0; i < 10; ++i) {
        expected.push_back(i);
    }
    expected.write(*ostream_);

    RVector<RInt32, 8> actual;
    actual.read(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}


TEST_F(RpcTypesTest, testRList)
{
    RList<RInt32, 7> expected;
    for (int i = 0; i < 10; ++i) {
        expected.push_back(i);
    }
    expected.write(*ostream_);

    RList<RInt32, 7> actual;
    actual.read(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}


TEST_F(RpcTypesTest, testRSet)
{
    RSet<RInt32> expected;
    for (int i = 0; i < 10; ++i) {
        expected.insert(i);
    }
    expected.write(*ostream_);

    RSet<RInt32> actual;
    actual.read(*istream_);
    EXPECT_EQ(
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    EXPECT_TRUE(std::equal(expected.begin(), expected.end(), actual.begin()));
}


TEST_F(RpcTypesTest, testEnum)
{
    enum EnumType { etA, etB, etC };
    typedef RpcIntType<EnumType, 3> REnumType;

    EnumType exp(etC);
    REnumType expected(exp);
    expected.write(*ostream_);

    REnumType actual;
    actual.read(*istream_);
    EnumType act = actual;
    EXPECT_EQ(expected, actual);
    EXPECT_EQ(exp, act);
}


TEST_F(RpcTypesTest, testRUserDefinedString)
{
    typedef RpcStringType<String, 10> RUserDefinedString;

    RUserDefinedString expected(String(12, 'X'));
    expected.write(*ostream_);

    RUserDefinedString actual;
    actual.read(*istream_);
    EXPECT_EQ(
        expected.size() - 2,
        static_cast<int>(actual.size()));
}


TEST_F(RpcTypesTest, testRMap)
{
    RMap<RInt32, RShortString> expected;
    for (int i = 0; i < 10; ++i) {
        OStringStream oss;
        oss << "#" << i;
        expected.insert(std::make_pair(i, oss.str()));
    }
    expected.write(*ostream_);

    RMap<RInt32, RShortString> actual;
    actual.read(*istream_);
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
    expected.write(*ostream_);

    RLimitedString actual;
    actual.read(*istream_);
    EXPECT_EQ(expected.substr(0, maxLength), actual.ref());
}
