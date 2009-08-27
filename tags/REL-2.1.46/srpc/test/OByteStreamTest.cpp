#include "stdafx.h"
#include "DummyStreamBuffer.h"
#include <srpc/detail/OByteStream.h>
#include <srpc/StreamFactory.h>

using namespace srpc;

/**
* @class OByteStreamTest
*
* output byte-stream Test
*/
class OByteStreamTest : public testing::Test
{
private:
    virtual void SetUp() {
        ostream_ = StreamFactory::createOStream(
            true, StreamFactory::stByte, buffer_).release();
    }

    virtual void TearDown() {
        delete ostream_;
    }

protected:
    DummyStreamBuffer buffer_;
    OStream* ostream_;
};


TEST_F(OByteStreamTest, testEmpty)
{
    EXPECT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteBit)
{
    ostream_->write(UInt8(1), 1);
    EXPECT_EQ(8, ostream_->getTotalBitCount());
    EXPECT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteBits)
{
    ostream_->write(UInt8(1), 1);
    EXPECT_EQ(1, ostream_->getTotalSize());

    ostream_->write(UInt8(0), 1);
    EXPECT_EQ(2, ostream_->getTotalSize());

    ostream_->write(UInt8(0), 7 + (8 - 2));
    EXPECT_EQ(3, ostream_->getTotalSize());

    ostream_->write(UInt8(1), 1);
    EXPECT_EQ(4, ostream_->getTotalSize());
    EXPECT_EQ(8 * 4, ostream_->getTotalBitCount());
}


TEST_F(OByteStreamTest, testWriteUnalignedBits)
{
    ostream_->write(UInt8(1), 9);
    EXPECT_EQ(1 * 8, ostream_->getTotalBitCount());
    EXPECT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int8(127));
    }
    EXPECT_EQ(8 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt8(255));
    }
    EXPECT_EQ(8 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(1 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int16(-1));
    }
    EXPECT_EQ(16 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt16(-1));
    }
    EXPECT_EQ(16 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int32(-1));
    }
    EXPECT_EQ(32 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt32(-1));
    }
    EXPECT_EQ(32 * 10 , ostream_->getTotalBitCount());
    EXPECT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteFloat32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(423423.65F);
    }
    EXPECT_EQ(32 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteString)
{
    const String s("0123456789");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(80 + 16, ostream_->getTotalBitCount());
    EXPECT_EQ(2 + 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteShortString)
{
    const String s("0123456789");
    ostream_->write(s, UCHAR_MAX, Bits<UInt8>::size);
    EXPECT_EQ(80 + 8, ostream_->getTotalBitCount());
    EXPECT_EQ(1 + 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testAlign)
{
    ostream_->write(UInt8(1), 1);
    ostream_->align();
    EXPECT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testReset)
{
    ostream_->write(UInt8(1), 1);
    ostream_->reset(false);
    EXPECT_EQ(1, ostream_->getTotalSize());

    ostream_->reset(true);
    EXPECT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testOutOfMemory)
{
    buffer_.setPushError();
    EXPECT_THROW(ostream_->write(UInt8(1), 1), Exception);
}


TEST_F(OByteStreamTest, testWriteZeroLengthString)
{
    const String s("");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(0 + 16, ostream_->getTotalBitCount());
    EXPECT_EQ(2 + 0, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteUInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt64(i + INT_MAX));
    }
    EXPECT_EQ(64 * 10 , ostream_->getTotalBitCount());
    EXPECT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(-Int64(i));
    }
    EXPECT_EQ(64 * 10 , ostream_->getTotalBitCount());
    EXPECT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OByteStreamTest, testWriteBuffer)
{
    char buffer[] = "¹«±ÃÈ­²ÉÀÌ ÇÇ¾ú½À´Ï´Ù.";
    const UInt16 length = static_cast<UInt16>(strlen(buffer));
    ostream_->write(buffer, length);

    EXPECT_EQ(length * CHAR_BIT, ostream_->getTotalBitCount());
    EXPECT_EQ(length, ostream_->getTotalSize());
}
