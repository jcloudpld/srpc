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
        EXPECT_TRUE(wcscmp(expected.c_str(),
            fromUtf8("You aren't gonna need it!").c_str()) == 0) <<
            "UTF-8 to UCS";
    }
}


TEST_F(UnicodeTest, testKorean)
{
#ifdef _MSC_VER
    {
        const String expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        EXPECT_EQ(expected, toUtf8(L"���d�d")) << "UCS to UTF-8";
    }
    {
        const WString expected(L"���d�d");
        EXPECT_EQ(expected, fromUtf8("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F")) <<
            "UTF-8 to UCS";
    }
#else
    // GCC�� ��� �����ڵ� ���ڿ��� �ٷ� �Է��� ��� �����Ϸ��� �����
    // �Ľ����� ���ϴ� ������ �߻��Ѵ�.
    {
        const String expected("\xEC\x95\x84\xED\x96\x8F\xED\x96\x8F");
        EXPECT_EQ(expected, toUtf8(fromUtf8(expected))) <<
            "utf-8 -> ucs -> utf-8";
    }
#endif
}


TEST_F(UnicodeTest, testInvalidUtf8)
{
    const WString str(L"1234567890ABCDEFGHIZKLMNOPQRSTUVWXYZ");

    const String utf8(toUtf8(str));
    const UInt32 strLen = 10;
    String invalidUtf8;
    for (String::size_type i = 0; i < strLen; ++i) {
        invalidUtf8.push_back(utf8[i]);
    }

    const WString converted(fromUtf8(invalidUtf8));
    EXPECT_TRUE(wcscmp(L"1234567890", converted.c_str()) == 0) <<
        "converting";
}
