#include "stdafx.h"
#include "StreamFixture.h"

using namespace srpc;

/**
* @class IByteStreamTest
*
* input bit-stream Test
*/
class IByteStreamTest : public ByteStreamFixture
{
};


TEST_F(IByteStreamTest, testEmpty)
{
    EXPECT_EQ(0, istream_->getTotalBitCount()) << "empty";
    EXPECT_EQ(0, istream_->getTotalSize()) << "empty";
}


TEST_F(IByteStreamTest, testReadBit)
{
    ostream_->write(UInt8(1), 1);

    EXPECT_EQ(8, istream_->getTotalBitCount());
    EXPECT_EQ(1, istream_->getTotalSize());

    UInt8 bit;
    istream_->read(bit, 1);
    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadBits)
{
    ostream_->write(UInt8(0), 1);
    ostream_->write(UInt8(1), 1);
    ostream_->write(UInt8(2), 2);
    ostream_->write(UInt8(3), 3);
    EXPECT_EQ(4, ostream_->getTotalSize());

    UInt8 bit;
    istream_->read(bit, 1);
    EXPECT_EQ(3, istream_->getTotalSize());
    EXPECT_EQ(0, bit);

    istream_->read(bit, 1);
    EXPECT_EQ(1, bit);

    istream_->read(bit, 2);
    EXPECT_EQ(2, bit);

    istream_->read(bit, 3);
    EXPECT_EQ(3, bit);

    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int8(i - 9));
    }

    EXPECT_EQ(8 * 10, istream_->getTotalBitCount());
    EXPECT_EQ(10, istream_->getTotalSize());

    Int8 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(i - 9, value);
    }
}


TEST_F(IByteStreamTest, testReadUInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt8(i));
    }

    EXPECT_EQ(8 * 10, istream_->getTotalBitCount());
    EXPECT_EQ(10, istream_->getTotalSize());

    UInt8 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(i, value);
    }
}


TEST_F(IByteStreamTest, testReadInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int16(i - 9));
    }

    EXPECT_EQ(16 * 10, istream_->getTotalBitCount());
    EXPECT_EQ(20, istream_->getTotalSize());

    Int16 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(i - 9, value);
    }
}


TEST_F(IByteStreamTest, testReadUInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt16(i));
    }

    EXPECT_EQ(16 * 10, istream_->getTotalBitCount());
    EXPECT_EQ(20, istream_->getTotalSize());

    UInt16 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(i, value);
    }
}



TEST_F(IByteStreamTest, testReadInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int32(i - 9));
    }

    EXPECT_EQ(32 * 10, istream_->getTotalBitCount());
    EXPECT_EQ(40, istream_->getTotalSize());

    Int32 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(i - 9, value);
    }
}


TEST_F(IByteStreamTest, testReadUInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt32(i));
    }

    EXPECT_EQ(4 * 10 * 8, istream_->getTotalBitCount());
    EXPECT_EQ(40, istream_->getTotalSize());

    UInt32 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(i, value);
    }
}


TEST_F(IByteStreamTest, testReadFloat32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Float32(i + 0.123));
    }

    EXPECT_EQ(32 * 10, istream_->getTotalBitCount());
    EXPECT_EQ(40, istream_->getTotalSize());

    Float32 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(Float32(i + 0.123), value);
    }
}


TEST_F(IByteStreamTest, testReadString)
{
    String original("0123456789");

    ostream_->write(original, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(2 + 10, ostream_->getTotalSize());

    String value;
    istream_->read(value, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(original, value);
    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadShortString)
{
    String original("0123456789");
    ostream_->write(original, UCHAR_MAX, Bits<UInt8>::size);
    EXPECT_EQ(1 + original.size(), ostream_->getTotalSize());

    String value;
    istream_->read(value, USHRT_MAX, Bits<UInt8>::size);
    EXPECT_EQ(original, value);
    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadWString)
{
    WString original(L"0123456789");

    ostream_->write(original, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(2 + 10, ostream_->getTotalSize());

    WString value;
    istream_->read(value, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_TRUE(original == value);
    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadWShortString)
{
    WString original(L"0123456789");
    ostream_->write(original, UCHAR_MAX, Bits<UInt8>::size);
    EXPECT_EQ(1 + original.size(), ostream_->getTotalSize());

    WString value;
    istream_->read(value, USHRT_MAX, Bits<UInt8>::size);
    EXPECT_TRUE(original == value);
    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testUnderflow)
{
    Int8 value = 12;
    ostream_->write(value);

    istream_->read(value);

    EXPECT_THROW(istream_->read(value), Exception);
}


TEST_F(IByteStreamTest, testAlign)
{
    ostream_->write(UInt8(0), 1);
    ostream_->write(UInt8(1), 1);

    UInt8 bit;
    istream_->read(bit, 1);
    istream_->read(bit, 1);

    istream_->align();

    EXPECT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(IByteStreamTest, testReadSignedInt)
{
    for (int i = 2; i <= 2; ++i) {
        ostream_->write(Int32(-(i - 1)), i);
    }

    EXPECT_EQ((2 - 2 + 1) * 4, ostream_->getTotalSize());

    Int32 value;
    for (int i = 2; i <= 2; ++i) {
        istream_->read(value, i);
        EXPECT_EQ(-(i - 1), value) << i << " bits";
    }

    EXPECT_EQ(0, istream_->getTotalBitCount());
}


TEST_F(IByteStreamTest, testReadZeroLengthString)
{
    String original("");
    ostream_->write(original, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(2 + 0, ostream_->getTotalSize());

    String value;
    istream_->read(value, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(original, value);
    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}


TEST_F(IByteStreamTest, testReadUInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt64(i) + INT_MAX);
    }

    EXPECT_EQ(8 * 10 * 8, istream_->getTotalBitCount());
    EXPECT_EQ(8 * 10, istream_->getTotalSize());

    UInt64 value;
    for (int i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(static_cast<UInt64>(i) + INT_MAX, value) << "#" << i << value;
    }
}


TEST_F(IByteStreamTest, testReadInt64)
{
    for (Int64 i = 0; i < 10; ++i) {
        ostream_->write(-i);
    }

    EXPECT_EQ(8 * 10 * 8, istream_->getTotalBitCount());
    EXPECT_EQ(8 * 10, istream_->getTotalSize());

    Int64 value;
    for (Int64 i = 0; i < 10; ++i) {
        istream_->read(value);
        EXPECT_EQ(-i, value) << "#" << i << "value";
    }
}


TEST_F(IByteStreamTest, testReadBuffer)
{
    char original[] = "0123456789";
    int length = static_cast<int>(strlen(original));

    ostream_->write(original, static_cast<UInt16>(length));
    EXPECT_EQ(length, ostream_->getTotalSize());

    char actual[10 + 1];
    istream_->read(actual, static_cast<UInt16>(length));
    actual[length] = '\0';
    EXPECT_EQ(length, static_cast<int>(strlen(actual))) << "buffer size";
    EXPECT_EQ(String(original), String(actual));
    EXPECT_EQ(0, istream_->getTotalBitCount());
    EXPECT_EQ(0, istream_->getTotalSize());
}
