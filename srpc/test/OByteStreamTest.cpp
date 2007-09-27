#include "stdafx.h"
#include "DummyStreamBuffer.h"
#include <srpc/detail/OByteStream.h>

using namespace srpc;

/**
* @class OByteStreamTest
*
* output byte-stream Test
*/
class OByteStreamTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(OByteStreamTest);
    CPPUNIT_TEST(testEmpty);
    CPPUNIT_TEST(testWriteBit);
    CPPUNIT_TEST(testWriteBits);
    CPPUNIT_TEST(testWriteUnalignedBits);
    CPPUNIT_TEST(testWriteInt8);
    CPPUNIT_TEST(testWriteUInt8);
    CPPUNIT_TEST(testWriteInt16);
    CPPUNIT_TEST(testWriteUInt16);
    CPPUNIT_TEST(testWriteInt32);
    CPPUNIT_TEST(testWriteUInt32);
    CPPUNIT_TEST(testWriteFloat32);
    CPPUNIT_TEST(testWriteString);
    CPPUNIT_TEST(testWriteShortString);
    CPPUNIT_TEST(testAlign);
    CPPUNIT_TEST(testReset);
    CPPUNIT_TEST(testOutOfMemory);
    CPPUNIT_TEST(testWriteZeroLengthString);
    CPPUNIT_TEST(testWriteUInt64);
    CPPUNIT_TEST(testWriteInt64);
    CPPUNIT_TEST(testWriteBuffer);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testEmpty();
    void testWriteBit();
    void testWriteBits();
    void testWriteUnalignedBits();
    void testWriteInt8();
    void testWriteUInt8();
    void testWriteInt16();
    void testWriteUInt16();
    void testWriteInt32();
    void testWriteUInt32();
    void testWriteFloat32();
    void testWriteSignedFloat32();
    void testWriteString();
    void testWriteShortString();
    void testAlign();
    void testReset();
    void testOutOfMemory();
    void testWriteZeroLengthString();
    void testWriteUInt64();
    void testWriteInt64();
    void testWriteBuffer();
private:
    DummyStreamBuffer buffer_;
    OStream* ostream_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(OByteStreamTest);

void OByteStreamTest::setUp()
{
    ostream_ = new OByteStream(buffer_);
}


void OByteStreamTest::tearDown()
{
    delete ostream_;
}


void OByteStreamTest::testEmpty()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteBit()
{
    ostream_->write(UInt8(1), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        8, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteBits()
{
    ostream_->write(UInt8(1), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());

    ostream_->write(UInt8(0), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 byte",
        2, ostream_->getTotalSize());

    ostream_->write(UInt8(0), 7 + (8 - 2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 byte",
        3, ostream_->getTotalSize());

    ostream_->write(UInt8(1), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 byte",
        4, ostream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("n byte",
        8 * 4, ostream_->getTotalBitCount());
}


void OByteStreamTest::testWriteUnalignedBits()
{
    ostream_->write(UInt8(1), 9);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 * 8 bit",
        1 * 8, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteInt8()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int8(127));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 bit",
        8 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 * 10 byte",
        10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteUInt8()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt8(255));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 bit",
        8 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 * 10 byte",
        1 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteInt16()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int16(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("16 * 10 bit",
        16 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 byte",
        2 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteUInt16()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt16(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("16 * 10 bit",
        16 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 byte",
        2 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteInt32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int32(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("32 * 10 bit",
        32 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 * 10 byte",
        4 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteUInt32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt32(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("32 * 10 bit",
        32 * 10 , ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 * 10 byte",
        4 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteFloat32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(423423.65F);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("32 * 10 bit",
        32 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 * 10 byte",
        4 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteString()
{
    const std::string s("0123456789");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("80 + 16 bit",
        80 + 16, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("10 + 2 byte",
        2 + 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteShortString()
{
    const std::string s("0123456789");
    ostream_->write(s, UCHAR_MAX, Bits<UInt8>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("80 + 8 bit",
        80 + 8, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("10 + 2 byte",
        1 + 10, ostream_->getTotalSize());
}


void OByteStreamTest::testAlign()
{
    ostream_->write(UInt8(1), 1);
    ostream_->align();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());
}


void OByteStreamTest::testReset()
{
    ostream_->write(UInt8(1), 1);
    ostream_->reset(false);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("one byte",
        1, ostream_->getTotalSize());

    ostream_->reset(true);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, ostream_->getTotalSize());
}


void OByteStreamTest::testOutOfMemory()
{
    buffer_.setPushError();
    try
    {
        ostream_->write(UInt8(1), 1);
        CPPUNIT_FAIL("exception is not thrown");
    }
    catch (const Exception&) {}
}


void OByteStreamTest::testWriteZeroLengthString()
{
    const std::string s("");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 + 16 bit",
        0 + 16, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 + 2 byte",
        2 + 0, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteUInt64()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt64(i + INT_MAX));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("64 * 10 bit",
        64 * 10 , ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 byte",
        8 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteInt64()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(-Int64(i));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("64 * 10 bit",
        64 * 10 , ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 byte",
        8 * 10, ostream_->getTotalSize());
}


void OByteStreamTest::testWriteBuffer()
{
    char buffer[] = "¹«±ÃÈ­²ÉÀÌ ÇÇ¾ú½À´Ï´Ù.";
    int length = static_cast<int>(strlen(buffer));
    ostream_->write(buffer, static_cast<UInt16>(length));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        length * CHAR_BIT, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bytes",
        length, ostream_->getTotalSize());
}
