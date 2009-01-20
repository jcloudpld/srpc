#include "stdafx.h"
#include <srpc/detail/IByteStream.h>
#include <srpc/utility/Unicode.h>
#include <srpc/StreamBuffer.h>
#include <srpc/Exception.h>

namespace srpc {

IByteStream::IByteStream(StreamBuffer& streamBuffer) :
    streamBuffer_(streamBuffer)
{
}


void IByteStream::read(void* buffer, UInt16 length)
{
    readBytes(buffer, length);
}


void IByteStream::read(String& value, size_t maxLength, int sizeBitCount)
{
    assert((sizeBitCount == 8) || (sizeBitCount == 16));

    value.clear();

    const UInt32 size = readStringSize(sizeBitCount);

    value.resize(size);
    readBytes(&(value[0]), size);

    if (value.size() > maxLength) {
        value.resize(maxLength);
    }
}


void IByteStream::read(WString& value, size_t maxLength, int sizeBitCount)
{
    assert((sizeBitCount == 8) || (sizeBitCount == 16));

    value.clear();

    const UInt32 size = readStringSize(sizeBitCount);

    String utf8;
    utf8.resize(size);
    readBytes(&(utf8[0]), size);

    value = fromUtf8(utf8);

    if (value.size() > maxLength) {
        value.resize(maxLength);
    }
}


UInt32 IByteStream::readStringSize(size_t sizeBitCount)
{
    if (sizeBitCount == 8) {
        return readByte();
    }
    
    UInt16 word = 0;
    readNumeric(word);
    return word;
}


void IByteStream::readBytes(void* buffer, size_t readSize)
{
    if (readSize > 0) {
        streamBuffer_.copyTo(static_cast<StreamBuffer::Item*>(buffer),
            readSize);
    }
}


UInt8 IByteStream::readByte()
{
    if (streamBuffer_.empty()) {
        throw StreamingException(__FILE__, __LINE__, "buffer is empty");
    }

    const UInt8 value = streamBuffer_.front();
    streamBuffer_.pop();
    return value;
}


void IByteStream::reset(bool resetBuffer)
{
    if (resetBuffer) {
        streamBuffer_.reset();
    }
}


int IByteStream::getTotalSize() const
{
    return static_cast<int>(streamBuffer_.size());
}

} // namespace srpc
