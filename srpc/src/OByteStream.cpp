#include "stdafx.h"
#include <srpc/detail/OByteStream.h>
#include <srpc/utility/Unicode.h>
#include <srpc/StreamBuffer.h>
#include <srpc/Exception.h>

namespace srpc {

// = OByteStream

OByteStream::OByteStream(StreamBuffer& streamBuffer,
    bool shouldUseUtf8ForString) :
    streamBuffer_(streamBuffer),
    shouldUseUtf8ForString_(shouldUseUtf8ForString)
{
}


void OByteStream::write(const String& value, size_t maxLength,
    int sizeBitCount)
{
    const UInt16 strLen =
        static_cast<UInt16>((std::min)(value.size(), maxLength));
    writeStringLength(strLen, sizeBitCount);
    writeBytes(value.data(), strLen);
}


void OByteStream::write(const WString& value, size_t maxLength,
    int sizeBitCount)
{
    WString realValue(value);
    if (realValue.size() > maxLength) {
        realValue.resize(maxLength);
    }

    if (shouldUseUtf8ForString_) {
        const String utf8(toUtf8(realValue));
        const UInt16 strLen = static_cast<UInt16>(utf8.size());
        writeStringLength(strLen, sizeBitCount);
        writeBytes(utf8.data(), utf8.size());
    }
    else {
        writeNumeric(static_cast<UInt32>(maxLength));

        WString::const_iterator pos = realValue.begin();
        const WString::const_iterator end = realValue.end();
        for (; pos != end; ++pos) {
            writeNumeric(static_cast<WString::value_type>(*pos));
        }
    }
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
