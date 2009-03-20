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
class OBitStreamTest : public testing::Test
{
private:
    virtual void SetUp() {
        ostream_ = StreamFactory::createOStream(StreamFactory::stBit, buffer_);
    }

    virtual void TearDown() {
        delete ostream_;
    }

protected:
    DummyStreamBuffer buffer_;
    OStream* ostream_;
};


TEST_F(OBitStreamTest, testEmpty)
{
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(0, ostream_->getTotalBitCount());
    EXPECT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteBit)
{
    ostream_->write(UInt8(1), 1);
    EXPECT_EQ(1, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(1, ostream_->getTotalBitCount());
    EXPECT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteBits)
{
    ostream_->write(UInt8(1), 1);
    EXPECT_EQ(1, ostream_->getTotalBitCount()) << "1 bit";
    EXPECT_EQ(1, ostream_->getTotalSize()) << "1 byte";

    ostream_->write(UInt8(0), 1);
    EXPECT_EQ(2, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(2, ostream_->getTotalBitCount());
    EXPECT_EQ(1, ostream_->getTotalSize());

    ostream_->write(UInt8(0), 7 + (8 - 2));
    EXPECT_EQ(7, static_cast<OBitStream*>(ostream_)->getHoldingBitCount()) <<
        "7 + 8 bits";
    EXPECT_EQ(7 + 8, ostream_->getTotalBitCount());
    EXPECT_EQ(2, ostream_->getTotalSize());

    ostream_->write(UInt8(1), 1);
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(16, ostream_->getTotalBitCount());
    EXPECT_EQ(2, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUnalignedBits)
{
    ostream_->write(UInt8(1), 9);
    EXPECT_EQ(1, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(9, ostream_->getTotalBitCount());
    EXPECT_EQ(2, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int8(127));
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(8 * 10, ostream_->getTotalBitCount()) << "8 * 10 bits";
    EXPECT_EQ(1 * 10, ostream_->getTotalSize()) << "10 bytes";
}


TEST_F(OBitStreamTest, testWriteUInt8)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt8(255));
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(8 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(1 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int16(-1));
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(16 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUInt16)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt16(-1));
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(16 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(2 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(Int32(-1));
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(32 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUInt32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt32(-1));
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(32 * 10 , ostream_->getTotalBitCount());
    EXPECT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteFloat32)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(423423.65F);
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(32 * 10, ostream_->getTotalBitCount());
    EXPECT_EQ(4 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteString)
{
    const String s("0123456789");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(80 + 16, ostream_->getTotalBitCount());
    EXPECT_EQ(2 + 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteShortString)
{
    const String s(256, 'X');
    ostream_->write(s, UCHAR_MAX, Bits<UInt8>::size);
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ((255 * 8) + 8, ostream_->getTotalBitCount());
    EXPECT_EQ(1 + 255, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testAlign)
{
    ostream_->write(UInt8(1), 1);
    ostream_->align();
    EXPECT_EQ(8, ostream_->getTotalBitCount());
    EXPECT_EQ(1, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testReset)
{
    ostream_->write(UInt8(1), 1);
    ostream_->reset(false);
    EXPECT_EQ(0, ostream_->getTotalBitCount());
    EXPECT_EQ(1, ostream_->getTotalSize());

    ostream_->reset(true);
    EXPECT_EQ(0, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testOutOfMemory)
{
    buffer_.setPushError();

    EXPECT_THROW(ostream_->write(UInt8(1), 1), Exception);
}


TEST_F(OBitStreamTest, testWriteZeroLengthString)
{
    const String s("");
    ostream_->write(s, USHRT_MAX, Bits<UInt16>::size);
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(0 + 16, ostream_->getTotalBitCount());
    EXPECT_EQ(2 + 0, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteUInt64)
{
    for (int i = 0; i < 10; ++i) {
        ostream_->write(UInt64(i + INT_MAX));
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(64 * 10 , ostream_->getTotalBitCount());
    EXPECT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteInt64)
{
    for (Int64 i = 0; i < 10; ++i) {
        ostream_->write(-i);
    }
    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(64 * 10 , ostream_->getTotalBitCount());
    EXPECT_EQ(8 * 10, ostream_->getTotalSize());
}


TEST_F(OBitStreamTest, testWriteBuffer)
{
    char buffer[] = "독도는 우리땅이라고!";
    const UInt16 length = static_cast<UInt16>(strlen(buffer));
    ostream_->write(buffer, length);

    EXPECT_EQ(0, static_cast<OBitStream*>(ostream_)->getHoldingBitCount());
    EXPECT_EQ(length * CHAR_BIT, ostream_->getTotalBitCount());
    EXPECT_EQ(length, ostream_->getTotalSize());
}
