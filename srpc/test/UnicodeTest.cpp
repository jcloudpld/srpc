#include "stdafx.h"
#include <srpc/utility/Unicode.h>
#include <srpc/Types.h>

using namespace srpc;

/**
* @class UnicodeTest 
*
* Unicode Test
*/
class UnicodeTest : public testing::Test
{
private:
    void testEnglish();
    void testKorean();
    void testInvalidUtf8();
};


TEST_F(UnicodeTest, testEnglish)
{
    {
        const String expected("You aren't gonna need it!");
        EXPECT_EQ(expected, toUtf8(L"You aren't gonna need it!")) <<
            "UCS to UTF-8";
    }
    {
        const WString expected(L"You aren't gonna need it!");
        EXPECT_EQ(expected, fromUtf8("You aren't gonna need it!")) <<
            "UTF-8 to UCS";
    }
}


TEST_F(UnicodeTest, testKorean)
{
#ifdef _MSC_VER
    {
        const String expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        EXPECT_EQ(expected, toUtf8(L"아햏햏")) << "UCS to UTF-8";
    }
    {
        const WString expected(L"아햏햏");
        EXPECT_EQ(expected, fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F")) <<
            "UTF-8 to UCS";
    }
#else
    // GCC의 경우 유니코드 문자열을 바로 입력할 경우 컴파일러가 제대로
    // 파싱하지 못하는 문제가 발생한다.
    {
        const String expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        EXPECT_EQ(expected, toUtf8(fromUtf8(expected))) <<
            "utf-8 -> ucs -> utf-8";
    }
#endif
}


TEST_F(UnicodeTest, testInvalidUtf8)
{
    const WString str(L"가나 다라 마바사 아자 차카타파하. 거너 더러머버서");

    const String utf8(toUtf8(str));
    const UInt32 strLen = 60;
    String invalidUtf8;
    for (String::size_type i = 0; i < strLen; ++i) {
        invalidUtf8.push_back(utf8[i]);
    }

    const WString converted(fromUtf8(invalidUtf8));
    EXPECT_EQ(
        WString(L"가나 다라 마바사 아자 차카타파하. 거너 더"), converted) <<
        "converting";
}
