#ifndef SRPC_IBYTESTREAM_H
#define SRPC_IBYTESTREAM_H

#include "../IStream.h"
#include "../utility/Endian.h"
#include "../config/config.h"

namespace srpc {

class StreamBuffer;

/** @addtogroup serialization
* @{
*/

/**
 * @class IByteStream
 *
 * 바이트 단위의 Input stream.
 */
class SRPC_API IByteStream : public IStream
{
public:
    IByteStream(StreamBuffer& streamBuffer);
private:
    virtual void read(UInt64& value, int /*bitCount*/) {
        readNumeric(value);
    }

    virtual void read(UInt32& value, int /*bitCount*/) {
        readNumeric(value);
    }

    virtual void read(UInt16& value, int /*bitCount*/) {
        readNumeric(value);
    }

    virtual void read(UInt8& value, int /*bitCount*/) {
        value = readByte();
    }

    virtual void read(Int64& value, int /*bitCount*/) {
        readNumeric(value);
    }

    virtual void read(Int32& value, int /*bitCount*/) {
        readNumeric(value);
    }

    virtual void read(Int16& value, int /*bitCount*/) {
        readNumeric(value);
    }

    virtual void read(Int8& value, int /*bitCount*/) {
        value = readByte();
    }

    virtual void read(bool& value, int /*bitCount*/) {
        const UInt8 numeric = readByte();
        value = (numeric != 0);
    }

    virtual void read(Float32& value, int /*bitCount*/) {
        readNumeric(*static_cast<UInt32*>(static_cast<void*>(&value)));
    }

    virtual void read(std::string& value, size_t maxLength, int sizeBitCount) {
        readString(value, maxLength, sizeBitCount);
    }

    virtual void read(std::wstring& value, size_t maxLength,
        int sizeBitCount) {
        readString(value, maxLength, sizeBitCount);
    }

    virtual void read(void* buffer, UInt16 length);

    virtual void align() {}

    virtual void reset(bool resetBuffer);

    virtual int getTotalBitCount() const {
        return getTotalSize() * CHAR_BIT;
    }

    virtual int getTotalSize() const;
private:
    template <typename T>
    void readNumeric(T& value, int byteCount = sizeof(T)) {
        assert((2 <= byteCount) && (byteCount <= 8));
        readBytes(&value, byteCount);
        value = toRpcByteOrder(value);
    }
    void readString(std::string& value, size_t maxLength, size_t sizeBitCount);
    void readString(std::wstring& value, size_t maxLength,
        size_t sizeBitCount);
    UInt32 readStringSize(size_t sizeBitCount);
    void readBytes(void* value, size_t byteCount);
    UInt8 readByte();
private:
    StreamBuffer& streamBuffer_;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_IBYTESTREAM_H
