#include "stdafx.h"
#include <srpc/utility/Unicode.h>

using namespace srpc;

/**
* @class UnicodeTest 
*
* Unicode Test
*/
class UnicodeTest  : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(UnicodeTest );
    CPPUNIT_TEST(testEnglish);
    CPPUNIT_TEST(testKorean);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testEnglish();
    void testKorean();
};

CPPUNIT_TEST_SUITE_REGISTRATION(UnicodeTest);

void UnicodeTest::setUp()
{
}


void UnicodeTest::tearDown()
{
}


void UnicodeTest::testEnglish()
{
    {
        const String expected("You aren't gonna need it!");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("UCS to UTF-8",
            expected, toUtf8(L"You aren't gonna need it!"));
    }
    {
        const WString expected(L"You aren't gonna need it!");
        CPPUNIT_ASSERT_MESSAGE("UTF-8 to UCS",
            expected == fromUtf8("You aren't gonna need it!"));
    }
}


void UnicodeTest::testKorean()
{
#ifdef _MSC_VER
    {
        const String expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("UCS to UTF-8",
            expected, toUtf8(L"아햏햏"));
    }
    {
        const WString expected(L"아햏햏");
        CPPUNIT_ASSERT_MESSAGE("UTF-8 to UCS",
            expected == fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F"));
    }
#else
    // GCC의 경우 유니코드 문자열을 바로 입력할 경우 컴파일러가 제대로
    // 파싱하지 못하는 문제가 발생한다.
    {
        const String expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        CPPUNIT_ASSERT_EQUAL_MESSAGE("utf-8 -> ucs -> utf-8",
            expected, toUtf8(fromUtf8(expected)));
    }
#endif
}
