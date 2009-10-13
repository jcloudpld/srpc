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

IBitStream::IBitStream(StreamBuffer& streamBuffer,
    bool shouldUseUtf8ForString) :
    streamBuffer_(streamBuffer),
    shouldUseUtf8ForString_(shouldUseUtf8ForString),
    bitReadCount_(0)
{
}


void IBitStream::read(void* buffer, UInt16 length)
{
    align();
    streamBuffer_.copyTo(
        static_cast<StreamBuffer::Item*>(buffer), length);
}


void IBitStream::readBits(UInt32& value, int bitCount)
{
    readBitsImpl(value, bitCount, streamBuffer_, bitReadCount_);
}


void IBitStream::readBits(UInt64& value, int bitCount)
{
    readBitsImpl(value, bitCount, streamBuffer_, bitReadCount_);
}


void IBitStream::read(String& value, size_t maxLength, int sizeBits)
{
    value.clear();

    UInt32 strLen;
    read(strLen, sizeBits);

    value.resize(strLen);
    read(&value[0], static_cast<UInt16>(strLen));

    if (value.size() > maxLength) {
        value.resize(maxLength);
    }
}


void IBitStream::read(WString& value, size_t maxLength, int sizeBits)
{
    value.clear();

    UInt32 strLen;
    read(strLen, sizeBits);

    if (shouldUseUtf8ForString_) {
        String utf8;
        utf8.resize(strLen);
        read(&utf8[0], static_cast<UInt16>(strLen));

        value = fromUtf8(utf8);
    }
    else {
        value.resize(strLen);
        read(&value[0],
            static_cast<UInt16>(strLen * sizeof(WString::value_type)));
    }

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
