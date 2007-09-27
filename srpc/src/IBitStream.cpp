#include "stdafx.h"
#include <srpc/detail/IBitStream.h>
#include <srpc/utility/Unicode.h>
#include <srpc/StreamBuffer.h>
#include <srpc/Exception.h>

namespace srpc {

namespace {

template <typename DataType>
void readBitsImpl(DataType& value, int bitCount,
    StreamBuffer& streamBuffer, int& bitReadCount)
{
    if (! isEnoughBits<DataType>(bitCount)) {
        throw StreamingException(__FILE__, __LINE__, "invalid bitCount");
    }

    int shiftCount = CHAR_BIT - bitReadCount;
    int leftBitCount = bitCount - shiftCount;
    const size_t needBytes = static_cast<size_t>(toBytes(leftBitCount) + 1);
    const size_t streamBufferSize = streamBuffer.size();
    if (streamBufferSize < needBytes) {
        throw StreamingException(__FILE__, __LINE__, "buffer is not enough");
    }

    DataType result =
        (static_cast<DataType>(streamBuffer.front()) >> bitReadCount);
    for (; leftBitCount > 0; leftBitCount -= CHAR_BIT) {
        streamBuffer.pop();
        result |= (static_cast<DataType>(streamBuffer.front()) << shiftCount);
        shiftCount += CHAR_BIT;
    }

    bitReadCount = (bitReadCount + bitCount) & (CHAR_BIT - 1);
    if (bitReadCount == 0) {
        streamBuffer.pop();
    }

    const DataType mask =
        (bitCount < Bits<DataType>::size) ? (1 << bitCount) - 1 : ~0;
    value = result & mask;
}

} // namespace

// = IBitStream

IBitStream::IBitStream(StreamBuffer& streamBuffer) :
    streamBuffer_(streamBuffer),
    bitReadCount_(0)
{
}


void IBitStream::read(void* buffer, UInt16 length)
{
    UInt8* byteBuffer = static_cast<UInt8*>(buffer);
    for (UInt16 i = 0; i < length; ++i) {
        byteBuffer[i] = readByte();
    }
}


void IBitStream::readBits(UInt32& value, int bitCount)
{
    readBitsImpl(value, bitCount, streamBuffer_, bitReadCount_);
}


void IBitStream::readBits(UInt64& value, int bitCount)
{
    readBitsImpl(value, bitCount, streamBuffer_, bitReadCount_);
}


void IBitStream::readString(std::string& value, size_t maxLength, int sizeBits)
{
    value.clear();

    UInt32 strLen;
    read(strLen, sizeBits);

    value.reserve(strLen);
    for (UInt32 i = 0; i < strLen; ++i) {
        value += static_cast<std::string::value_type>(readByte());
    }

    if (value.size() > maxLength) {
        value.resize(maxLength);
    }
}


void IBitStream::readString(std::wstring& value, size_t maxLength,
    int sizeBits)
{
    value.clear();

    UInt32 strLen;
    read(strLen, sizeBits);

    std::string utf8;
    utf8.reserve(strLen);
    for (UInt32 i = 0; i < strLen; ++i) {
        utf8 += static_cast<std::string::value_type>(readByte());
    }

    value = fromUtf8(utf8);
    if (value.size() > maxLength) {
        value.resize(maxLength);
    }
}


void IBitStream::align()
{
    if (bitReadCount_ > 0) {
        streamBuffer_.pop();
    }
    bitReadCount_ = 0;
}


void IBitStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
    bitReadCount_ = 0;
}


int IBitStream::getTotalBitCount() const
{
    return static_cast<int>(streamBuffer_.size()) * CHAR_BIT;
}

} // namespace srpc
