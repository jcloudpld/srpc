#include "stdafx.h"
#include <srpc/detail/OByteStream.h>
#include <srpc/utility/Unicode.h>
#include <srpc/StreamBuffer.h>
#include <srpc/Exception.h>

namespace srpc {

// = OByteStream

OByteStream::OByteStream(StreamBuffer& streamBuffer) :
    streamBuffer_(streamBuffer)
{
}


void OByteStream::write(const std::string& value, size_t maxLength,
    int sizeBitCount)
{
    const UInt16 strLen =
        static_cast<UInt16>((std::min)(value.size(), maxLength));
    writeStringLength(strLen, sizeBitCount);
    writeBytes(value.data(), strLen);
}


void OByteStream::write(const std::wstring& value, size_t maxLength,
    int sizeBitCount)
{
    std::wstring realValue(value);
    if (realValue.size() > maxLength) {
        realValue.resize(maxLength);
    }
    const std::string utf8(toUtf8(realValue));
    const UInt16 strLen = static_cast<UInt16>(utf8.size());
    writeStringLength(strLen, sizeBitCount);
    writeBytes(utf8.data(), utf8.size());
}


void OByteStream::write(const void* buffer, UInt16 length)
{
    writeBytes(buffer, length);
}


void OByteStream::writeStringLength(UInt16 strLen, int sizeBitCount)
{
    assert((sizeBitCount == 8) || (sizeBitCount == 16));
    if (sizeBitCount <= 8) {
        writeNumeric(static_cast<UInt8>(strLen));
    }
    else {
        writeNumeric(strLen);
    }
}


void OByteStream::writeBytes(const void* value, size_t byteCount)
{
    streamBuffer_.copyFrom(static_cast<const StreamBuffer::Item*>(value),
        byteCount);
}


void OByteStream::writeByte(UInt8 value)
{
    streamBuffer_.push(value);
}


void OByteStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
}


int OByteStream::getTotalSize() const
{
    return static_cast<int>(streamBuffer_.size());
}

} // namespace srpc
