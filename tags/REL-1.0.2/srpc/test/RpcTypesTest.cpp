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
* @todo 64비트 정수(실수) 추가
*/
class RpcTypesTest : public BitStreamTexture
{
    CPPUNIT_TEST_SUITE(RpcTypesTest );
    CPPUNIT_TEST(testRInt8);
    CPPUNIT_TEST(testRUInt8);
    CPPUNIT_TEST(testRInt16);
    CPPUNIT_TEST(testRUInt16);
    CPPUNIT_TEST(testRInt32);
    CPPUNIT_TEST(testRUInt32);
    CPPUNIT_TEST(testRInt64);
    CPPUNIT_TEST(testRUInt64);
    CPPUNIT_TEST(testRFloat32);
    CPPUNIT_TEST(testRString);
    CPPUNIT_TEST(testRShortString);
    CPPUNIT_TEST(testRBool);
    CPPUNIT_TEST(testRRpcId);
    CPPUNIT_TEST(testRWString);
    CPPUNIT_TEST(testRWShortString);
    CPPUNIT_TEST(testRVector);
    CPPUNIT_TEST(testRList);
    CPPUNIT_TEST(testRSet);
    CPPUNIT_TEST(testEnum);
    CPPUNIT_TEST(testRUserDefinedString);
    CPPUNIT_TEST(testRMap);
    CPPUNIT_TEST_SUITE_END();
private:
    void testRInt8();
    void testRUInt8();
    void testRInt16();
    void testRUInt16();
    void testRInt32();
    void testRUInt32();
    void testRInt64();
    void testRUInt64();
    void testRFloat32();
    void testRString();
    void testRShortString();
    void testRBool();
    void testRRpcId();
    void testRWString();
    void testRWShortString();
    void testRVector();
    void testRList();
    void testRSet();
    void testEnum();
    void testRUserDefinedString();
    void testRMap();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcTypesTest );

void RpcTypesTest::testRInt8()
{
    RInt8 expected = SCHAR_MIN;
    expected.write(*ostream_);

    RInt8 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RInt8",
        static_cast<int>(expected), static_cast<int>(actual));
}


void RpcTypesTest::testRUInt8()
{
    RUInt8 expected = SCHAR_MAX;
    expected.write(*ostream_);

    RUInt8 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RUInt8",
        expected, actual);
}


void RpcTypesTest::testRInt16()
{
    RInt16 expected = SHRT_MIN;
    expected.write(*ostream_);

    RInt16 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RInt16",
        expected, actual);
}


void RpcTypesTest::testRUInt16()
{
    RUInt16 expected = SHRT_MAX;
    expected.write(*ostream_);

    RUInt16 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RUInt16",
        expected, actual);
}


void RpcTypesTest::testRInt32()
{
    RInt32 expected = INT_MIN;
    expected.write(*ostream_);

    RInt32 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RInt32",
        expected, actual);
}


void RpcTypesTest::testRUInt32()
{
    RUInt32 expected = INT_MAX;
    expected.write(*ostream_);

    RUInt32 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RUInt32",
        expected, actual);
}


void RpcTypesTest::testRInt64()
{
    RInt64 expected = -1;
    expected.write(*ostream_);

    RInt64 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RInt64",
        expected, actual);
}


void RpcTypesTest::testRUInt64()
{
    RUInt64 expected = static_cast<UInt64>(-1);
    expected.write(*ostream_);

    RUInt64 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RUInt64",
        expected, actual);
}


void RpcTypesTest::testRFloat32()
{
    RFloat32 expected = -123.456F;
    expected.write(*ostream_);

    RFloat32 actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RFloat32",
        expected, actual);
}


void RpcTypesTest::testRString()
{
    RString expected = "무궁화 꽃이 피었습니다.";
    expected.write(*ostream_);

    RString actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RString",
        expected, actual);
}


void RpcTypesTest::testRShortString()
{
    RShortString expected(std::string(256, 'X'));
    expected.write(*ostream_);

    RShortString actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RShortString",
        static_cast<int>(expected.size() - 1),
        static_cast<int>(actual.size()));
}


void RpcTypesTest::testRBool()
{
    {
        RBool expected = false;
        expected.write(*ostream_);

        RBool actual;
        actual.read(*istream_);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("RBool",
            expected, actual);
    }
    {
        RBool expected = true;
        expected.write(*ostream_);

        RBool actual;
        actual.read(*istream_);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("RBool",
            expected, actual);
    }
}


void RpcTypesTest::testRRpcId()
{
    RpcId expected("RpcId");
    expected.write(*ostream_);

    RpcId actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RpcId",
        expected.get(), actual.get());
}


void RpcTypesTest::testRWString()
{
    RWString expected(fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.write(*ostream_);

    RWString actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_MESSAGE("RWString",
        expected == actual);
}


void RpcTypesTest::testRWShortString()
{
    RWShortString expected(fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    expected.write(*ostream_);

    RWShortString actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_MESSAGE("RWShortString",
        expected == actual);
}


void RpcTypesTest::testRVector()
{
    RVector<RInt32, 8> expected;
    for (int i = 0; i < 10; ++i) {
        expected.push_back(i);
    }
    expected.write(*ostream_);

    RVector<RInt32, 8> actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("equal",
        true,
        std::equal(expected.begin(), expected.end(), actual.begin()));
}


void RpcTypesTest::testRList()
{
    RList<RInt32, 7> expected;
    for (int i = 0; i < 10; ++i) {
        expected.push_back(i);
    }
    expected.write(*ostream_);

    RList<RInt32, 7> actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("equal",
        true,
        std::equal(expected.begin(), expected.end(), actual.begin()));
}


void RpcTypesTest::testRSet()
{
    RSet<RInt32> expected;
    for (int i = 0; i < 10; ++i) {
        expected.insert(i);
    }
    expected.write(*ostream_);

    RSet<RInt32> actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("equal",
        true,
        std::equal(expected.begin(), expected.end(), actual.begin()));
}


enum EnumType
{
    etA, etB, etC
};

void RpcTypesTest::testEnum()
{
    typedef RpcIntType<EnumType, 3> REnumType;

    EnumType exp(etC);
    REnumType expected(exp);
    expected.write(*ostream_);

    REnumType actual;
    actual.read(*istream_);
    EnumType act = actual;
    CPPUNIT_ASSERT_EQUAL_MESSAGE("enum",
        expected, actual);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("enum",
        exp, act);
}


void RpcTypesTest::testRUserDefinedString()
{
    typedef RpcStringType<std::string, 10> RUserDefinedString;

    RUserDefinedString expected(std::string(12, 'X'));
    expected.write(*ostream_);

    RUserDefinedString actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("RShortString",
        static_cast<int>(expected.size() - 2),
        static_cast<int>(actual.size()));
}


void RpcTypesTest::testRMap()
{
    RMap<RInt32, RShortString> expected;
    for (int i = 0; i < 10; ++i) {
        std::ostringstream oss;
        oss << "#" << i;
        expected.insert(std::make_pair(i, oss.str()));
    }
    expected.write(*ostream_);

    RMap<RInt32, RShortString> actual;
    actual.read(*istream_);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()),
        static_cast<int>(actual.size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("equal",
        true,
        std::equal(expected.begin(), expected.end(), actual.begin()));
}
