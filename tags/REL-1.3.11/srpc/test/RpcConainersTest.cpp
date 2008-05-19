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
    CPPUNIT_TEST_SUITE(RpcConainersTest );
    CPPUNIT_TEST(testRVector);
    CPPUNIT_TEST(testRList);
    CPPUNIT_TEST(testRSet);
    CPPUNIT_TEST(testRMap);
    CPPUNIT_TEST_SUITE_END();
private:
    void testRVector();
    void testRList();
    void testRSet();
    void testRMap();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcConainersTest );

void RpcConainersTest::testRVector()
{
    RVector<RInt32> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.write(*ostream_);

    RVector<RInt32> actual;
    actual.read(*istream_);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()), static_cast<int>(actual.size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last",
        expected.back(), actual.back());
}


void RpcConainersTest::testRList()
{
    RList<RInt32> expected;
    expected.push_back(1);
    expected.push_back(3);
    expected.push_back(5);
    expected.write(*ostream_);

    RList<RInt32> actual;
    actual.read(*istream_);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()), static_cast<int>(actual.size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("last",
        expected.back(), actual.back());
}


void RpcConainersTest::testRSet()
{
    RSet<RShortString> expected;
    expected.insert("1234");
    expected.insert("abcd");
    expected.insert("4567");
    expected.write(*ostream_);

    RSet<RShortString> actual;
    actual.read(*istream_);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()), static_cast<int>(actual.size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("first",
        *expected.begin(), *actual.begin());
}


void RpcConainersTest::testRMap()
{
    typedef RMap<RInt32, RShortString> MapType;
    MapType expected;
    expected.insert(MapType::value_type(3, "1234"));
    expected.insert(MapType::value_type(1, "1234"));
    expected.insert(MapType::value_type(2, "1234"));
    expected.write(*ostream_);

    MapType actual;
    actual.read(*istream_);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(expected.size()), static_cast<int>(actual.size()));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("first",
        (*expected.begin()).second, (*actual.begin()).second);
}
