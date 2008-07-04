#include "stdafx.h"
#include "DummyStreamBuffer.h"
#include <srpc/detail/OBitStream.h>
#include <srpc/StreamFactory.h>

using namespace srpc;

/**
* @class OBitStreamTest
*
* output bit-stream Test
*/
class OBitStreamTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(OBitStreamTest);
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

CPPUNIT_TEST_SUITE_REGISTRATION(OBitStreamTest);

void OBitStreamTest::setUp()
{
    ostream_ = StreamFactory::createOStream(StreamFactory::stBit, buffer_);
}


void OBitStreamTest::tearDown()
{
    delete ostream_;
}


void OBitStreamTest::testEmpty()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteBit()
{
    ostream_->write(UInt8(1), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 bit",
        1, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 bit",
        1, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteBits()
{
    ostream_->write(UInt8(1), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 bit",
        1, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());

    ostream_->write(UInt8(0), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 bit",
        2, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 bit",
        2, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());

    ostream_->write(UInt8(0), 7 + (8 - 2));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("7 + 8 bit",
        7, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("7 + 8 bit",
        7 + 8, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 byte",
        2, ostream_->getTotalSize());

    ostream_->write(UInt8(1), 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        16, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        2, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteUnalignedBits()
{
    ostream_->write(UInt8(1), 9);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 bit",
        1, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 bit",
        9, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        2, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteInt8()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int8(127));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 bit",
        8 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 * 10 byte",
        1 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteUInt8()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt8(255));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 bit",
        8 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 * 10 byte",
        1 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteInt16()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int16(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("16 * 10 bit",
        16 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 byte",
        2 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteUInt16()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt16(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("16 * 10 bit",
        16 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 byte",
        2 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteInt32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int32(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("32 * 10 bit",
        32 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 * 10 byte",
        4 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteUInt32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt32(-1));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("32 * 10 bit",
        32 * 10 , ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 * 10 byte",
        4 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteFloat32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(423423.65F);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("32 * 10 bit",
        32 * 10, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 * 10 byte",
        4 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteString()
{
    const String s("0123456789");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("80 + 16 bit",
        80 + 16, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("10 + 2 byte",
        2 + 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteShortString()
{
    const String s(256, 'X');
    ostream_->write(s, UCHAR_MAX, Bits<UInt8>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("255 * 8 + 8 bit",
        (255 * 8) + 8, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("255 + 1 byte",
        1 + 255, ostream_->getTotalSize());
}


void OBitStreamTest::testAlign()
{
    ostream_->write(UInt8(1), 1);
    ostream_->align();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 bit",
        8, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 byte",
        1, ostream_->getTotalSize());
}


void OBitStreamTest::testReset()
{
    ostream_->write(UInt8(1), 1);
    ostream_->reset(false);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("one byte",
        1, ostream_->getTotalSize());

    ostream_->reset(true);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, ostream_->getTotalSize());
}


void OBitStreamTest::testOutOfMemory()
{
    buffer_.setPushError();

    try {
        ostream_->write(UInt8(1), 1);
        CPPUNIT_FAIL("exception is not thrown");
    }
    catch (const Exception&) {}
}


void OBitStreamTest::testWriteZeroLengthString()
{
    const String s("");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 + 16 bit",
        0 + 16, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 + 2 byte",
        2 + 0, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteUInt64()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt64(i + INT_MAX));
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("64 * 10 bit",
        64 * 10 , ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 byte",
        8 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteInt64()
{
    for (Int64 i = 0; i < 10; ++i) {
        ostream_->write(-i);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("64 * 10 bit",
        64 * 10 , ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("8 * 10 byte",
        8 * 10, ostream_->getTotalSize());
}


void OBitStreamTest::testWriteBuffer()
{
    char buffer[] = "¹«±ÃÈ­²ÉÀÌ ÇÇ¾ú½À´Ï´Ù.";
    int length = static_cast<int>(strlen(buffer));
    ostream_->write(buffer, static_cast<UInt16>(length));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 bit",
        0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        length * CHAR_BIT, ostream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bytes",
        length, ostream_->getTotalSize());
}
