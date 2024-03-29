#ifndef SRPC_OBYTESTREAM_H
#define SRPC_OBYTESTREAM_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../OStream.h"
#include "../utility/Endian.h"

namespace srpc {

class StreamBuffer;

/** @addtogroup serialization
* @{
*/

/**
 * @class OByteStream
 *
 * 바이트 단위의 Output stream.
 */
class OByteStream : public OStream
{
public:
    OByteStream(StreamBuffer& streamBuffer, bool shouldUseUtf8ForString);
private:
    virtual void write(UInt64 value, int /*bitCount*/) {
        writeNumeric(value);
    }

    virtual void write(UInt32 value, int /*bitCount*/) {
        writeNumeric(value);
    }

    virtual void write(UInt16 value, int /*bitCount*/) {
        writeNumeric(value);
    }

    virtual void write(UInt8 value, int /*bitCount*/) {
        writeByte(value);
    }

    virtual void write(bool value, int /*bitCount*/) {
        writeByte(UInt8(value));
    }

    virtual void write(Int64 value, int /*bitCount*/) {
        writeNumeric(value);
    }

    virtual void write(Int32 value, int /*bitCount*/) {
        writeNumeric(value);
    }

    virtual void write(Int16 value, int /*bitCount*/) {
        writeNumeric(value);
    }

    virtual void write(Int8 value, int /*bitCount*/) {
        writeByte(value);
    }

    virtual void write(Float32 value, int /*bitCount*/) {
        writeNumeric(*static_cast<UInt32*>(static_cast<void*>(&value)));
    }

    virtual void write(const String& value, size_t maxLength,
        int sizeBitCount);

    virtual void write(const WString& value, size_t maxLength,
        int sizeBitCount);

    virtual void write(const void* buffer, UInt16 length);

    virtual void align() {}
    virtual void reset(bool resetBuffer);

    virtual int getTotalBitCount() const {
        return getTotalSize() * CHAR_BIT;
    }

    virtual int getTotalSize() const;
private:
    template <typename T>
    void writeNumeric(T value) {
        value = toRpcByteOrder(value);
        writeBytes(&value, sizeof(value));
    }
    void writeStringLength(UInt16 strLen, int sizeBitCount);
    void writeBytes(const void* value, size_t byteCount);
    void writeByte(UInt8 value);
private:
    StreamBuffer& streamBuffer_;
    const bool shouldUseUtf8ForString_;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_OBYTESTREAM_H
