#include "stdafx.h"
#include "StreamTexture.h"

using namespace srpc;

/**
* @class IByteStreamTest
*
* input bit-stream Test
*/
class IByteStreamTest : public ByteStreamTexture
{
    CPPUNIT_TEST_SUITE(IByteStreamTest);
    CPPUNIT_TEST(testEmpty);
    CPPUNIT_TEST(testReadBit);
    CPPUNIT_TEST(testReadBits);
    CPPUNIT_TEST(testReadInt8);
    CPPUNIT_TEST(testReadUInt8);
    CPPUNIT_TEST(testReadInt16);
    CPPUNIT_TEST(testReadUInt16);
    CPPUNIT_TEST(testReadInt32);
    CPPUNIT_TEST(testReadUInt32);
    CPPUNIT_TEST(testReadFloat32);
    CPPUNIT_TEST(testReadString);
    CPPUNIT_TEST(testReadShortString);
    CPPUNIT_TEST(testReadWString);
    CPPUNIT_TEST(testReadWShortString);
    CPPUNIT_TEST(testUnderflow);
    CPPUNIT_TEST(testAlign);
    CPPUNIT_TEST(testReadSignedInt);
    CPPUNIT_TEST(testReadZeroLengthString);
    CPPUNIT_TEST(testReadUInt64);
    CPPUNIT_TEST(testReadInt64);
    CPPUNIT_TEST(testReadBuffer);
    CPPUNIT_TEST_SUITE_END();
private:
    void testEmpty();
    void testReadBit();
    void testReadBits();
    void testReadInt8();
    void testReadUInt8();
    void testReadInt16();
    void testReadUInt16();
    void testReadInt32();
    void testReadUInt32();
    void testReadFloat32();
    void testReadString();
    void testReadShortString();
    void testReadWString();
    void testReadWShortString();
    void testUnderflow();
    void testAlign();
    void testReadSignedInt();
    void testReadZeroLengthString();
    void testReadUInt64();
    void testReadInt64();
    void testReadBuffer();
};

CPPUNIT_TEST_SUITE_REGISTRATION(IByteStreamTest);

void IByteStreamTest::testEmpty()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testReadBit()
{
    ostream_->write(UInt8(1), 1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        8, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        1, istream_->getTotalSize());

    UInt8 bit;
    istream_->read(bit, 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testReadBits()
{
    ostream_->write(UInt8(0), 1);
    ostream_->write(UInt8(1), 1);
    ostream_->write(UInt8(2), 2);
    ostream_->write(UInt8(3), 3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("4 byte",
        4, ostream_->getTotalSize());

    UInt8 bit;
    istream_->read(bit, 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        3, istream_->getTotalSize());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("bit",
        0, static_cast<int>(bit));

    istream_->read(bit, 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("bit",
        1, static_cast<int>(bit));

    istream_->read(bit, 2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("bit",
        2, static_cast<int>(bit));

    istream_->read(bit, 3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("bit",
        3, static_cast<int>(bit));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testReadInt8()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int8(i - 9));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        8 * 10, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        10, istream_->getTotalSize());

    Int8 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i - 9, static_cast<int>(value));
    }
}


void IByteStreamTest::testReadUInt8()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt8(i));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        8 * 10, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        10, istream_->getTotalSize());

    UInt8 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, static_cast<int>(value));
    }
}


void IByteStreamTest::testReadInt16()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int16(i - 9));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        16 * 10, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        20, istream_->getTotalSize());

    Int16 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i - 9, static_cast<int>(value));
    }
}


void IByteStreamTest::testReadUInt16()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt16(i));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        16 * 10, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        20, istream_->getTotalSize());

    UInt16 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, static_cast<int>(value));
    }
}



void IByteStreamTest::testReadInt32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int32(i - 9));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        32 * 10, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        40, istream_->getTotalSize());

    Int32 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i - 9, static_cast<int>(value));
    }
}


void IByteStreamTest::testReadUInt32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt32(i));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        4 * 10 * 8, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        40, istream_->getTotalSize());

    UInt32 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            i, static_cast<int>(value));
    }
}


void IByteStreamTest::testReadFloat32()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Float32(i + 0.123));
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        32 * 10, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        40, istream_->getTotalSize());

    Float32 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
            Float32(i + 0.123), value);
    }
}


void IByteStreamTest::testReadString()
{
    String original("0123456789");

    ostream_->write(original, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("10 + 2 byte",
        2 + 10, ostream_->getTotalSize());

    String value;
    istream_->read(value, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
        original, value);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testReadShortString()
{
    String original("0123456789");
    ostream_->write(original, UCHAR_MAX, Bits<UInt8>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        static_cast<int>(1 + original.size()), ostream_->getTotalSize());

    String value;
    istream_->read(value, USHRT_MAX, Bits<UInt8>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
        original, value);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testReadWString()
{
    WString original(L"0123456789");

    ostream_->write(original, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("10 + 2 byte",
        2 + 10, ostream_->getTotalSize());

    WString value;
    istream_->read(value, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_MESSAGE("value",
        original == value);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testReadWShortString()
{
    WString original(L"0123456789");
    ostream_->write(original, UCHAR_MAX, Bits<UInt8>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        static_cast<int>(1 + original.size()), ostream_->getTotalSize());

    WString value;
    istream_->read(value, USHRT_MAX, Bits<UInt8>::size);
    CPPUNIT_ASSERT_MESSAGE("value",
        original == value);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testUnderflow()
{
    Int8 value = 12;
    ostream_->write(value);

    istream_->read(value);

    try {
        istream_->read(value);
        CPPUNIT_FAIL("exception is not thrown");
    }
    catch (const Exception&) {}
}


void IByteStreamTest::testAlign()
{
    ostream_->write(UInt8(0), 1);
    ostream_->write(UInt8(1), 1);

    UInt8 bit;
    istream_->read(bit, 1);
    istream_->read(bit, 1);

    istream_->align();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
}


void IByteStreamTest::testReadSignedInt()
{
    for (int i = 2; i <= 2; ++i) {
        ostream_->write(Int32(-(i - 1)), i);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        (2 - 2 + 1) * 4, ostream_->getTotalSize());

    Int32 value;
    for (int i = 2; i <= 2; ++i) {
        istream_->read(value, i);
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("%d bits") % i).str(),
            -(i - 1), value);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
}


void IByteStreamTest::testReadZeroLengthString()
{
    String original("");
    ostream_->write(original, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 + 0 bytes",
        2 + 0, ostream_->getTotalSize());

    String value;
    istream_->read(value, USHRT_MAX, Bits<UInt16>::size);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
        original, value);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}


void IByteStreamTest::testReadUInt64()
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt64(i) + INT_MAX);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        8 * 10 * 8, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        8 * 10, istream_->getTotalSize());

    UInt64 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%d value") % i).str(),
            static_cast<UInt64>(i) + INT_MAX, value);
    }
}


void IByteStreamTest::testReadInt64()
{
    for (Int64 i = 0; i < 10; ++i) {
        ostream_->write(-i);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        8 * 10 * 8, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        8 * 10, istream_->getTotalSize());

    Int64 value;
    for (Int64 i = 0; i < 10; ++i) {
        istream_->read(value);
        CPPUNIT_ASSERT_EQUAL_MESSAGE((boost::format("#%d value") % i).str(),
            -i, value);
    }
}


void IByteStreamTest::testReadBuffer()
{
    char original[] = "0123456789";
    int length = static_cast<int>(strlen(original));

    ostream_->write(original, static_cast<UInt16>(length));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte count",
        length, ostream_->getTotalSize());

    char actual[10 + 1];
    istream_->read(actual, static_cast<UInt16>(length));
    actual[length] = '\0';
    CPPUNIT_ASSERT_EQUAL_MESSAGE("buffer size",
        length, static_cast<int>(strlen(actual)));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("buffer",
        String(original), String(actual));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total bit count",
        0, istream_->getTotalBitCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("total byte",
        0, istream_->getTotalSize());
}
