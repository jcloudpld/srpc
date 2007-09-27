#include "stdafx.h"
#include <srpc/detail/OBitStream.h>
#include <srpc/utility/Unicode.h>
#include <srpc/StreamBuffer.h>
#include <srpc/Exception.h>

namespace srpc {

namespace {

template <typename DataType>
inline DataType resetUnusedBits(DataType value, int bitCount)
{
    if (bitCount < Bits<DataType>::size) {
        return value & ((static_cast<DataType>(1) << bitCount) - 1);
    }
    return value;
}


template <typename DataType>
void writeBitsImpl(DataType value, int bitCount,
    StreamBuffer& streamBuffer, int& holdingBitCount, int& totalBitCount)
{
    assert(isEnoughBits<DataType>(bitCount));

    if (! isEnoughBits<DataType>(bitCount)) {
        throw StreamingException(__FILE__, __LINE__, "invalid bitCount");
    }

    DataType theValue = resetUnusedBits(value, bitCount);
    if (holdingBitCount > 0) {
        if (streamBuffer.empty()) {
            throw StreamingException(__FILE__, __LINE__, "buffer is empty");
        }
        streamBuffer.back() |= UInt8(theValue << holdingBitCount);
    }
    else {
        streamBuffer.push(UInt8(theValue));
    }

    const int bitShiftCount = CHAR_BIT - holdingBitCount;
    if (bitCount > bitShiftCount) {
        int leftBitCount = bitCount - bitShiftCount;
        theValue >>= bitShiftCount;
        do {
            streamBuffer.push(UInt8(theValue));
            theValue >>= CHAR_BIT;
            leftBitCount -= CHAR_BIT;
        }
        while (leftBitCount > 0);
    }
    holdingBitCount = (holdingBitCount + bitCount) & (CHAR_BIT - 1);

    totalBitCount += bitCount;
}

} // namespace

// = OBitStream

OBitStream::OBitStream(StreamBuffer& streamBuffer) :
    streamBuffer_(streamBuffer),
    holdingBitCount_(0),
    totalBitCount_(0)
{
}


void OBitStream::writeBits(UInt32 value, int bitCount)
{
    writeBitsImpl(value, bitCount, streamBuffer_, holdingBitCount_,
        totalBitCount_);
}


void OBitStream::writeBits(UInt64 value, int bitCount)
{
    writeBitsImpl(value, bitCount, streamBuffer_, holdingBitCount_,
        totalBitCount_);
}


void OBitStream::align()
{
    if (holdingBitCount_ > 0) {
        assert(getTotalSize() > 0);
        writeBits(UInt32(0), CHAR_BIT - holdingBitCount_);
        assert(0 == holdingBitCount_);
    }
}


void OBitStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
    totalBitCount_ = 0;
}


void OBitStream::write(const std::string& value, size_t maxLength,
    int sizeBitCount)
{
    const UInt32 strLen =
        static_cast<UInt32>((std::min)(value.size(), maxLength));
    write(strLen, sizeBitCount);
    for (std::string::size_type i = 0; i < strLen; ++i) {
        writeByte(value[i]);
    }
}


void OBitStream::write(const std::wstring& value, size_t maxLength,
    int sizeBitCount)
{
    std::wstring realValue = value;
    if (value.size() > maxLength) {
        realValue.resize(maxLength);
    }
    const std::string utf8(toUtf8(realValue));
    const UInt32 strLen =
        static_cast<UInt32>((std::min)(utf8.size(), maxLength));
    write(strLen, sizeBitCount);
    for (std::string::size_type i = 0; i < strLen; ++i) {
        writeByte(utf8[i]);
    }
}


void OBitStream::write(const void* buffer, UInt16 length)
{
    const UInt8* byteBuffer = static_cast<const UInt8*>(buffer);
    for (UInt16 i = 0; i < length; ++i) {
        writeByte(byteBuffer[i]);
    }
}


int OBitStream::getTotalSize() const
{
    return static_cast<int>(streamBuffer_.size());
}

} // namespace srpc
