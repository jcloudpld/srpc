#ifndef SRPC_IBITSTREAM_H
#define SRPC_IBITSTREAM_H

#include "../IStream.h"

namespace srpc {

class StreamBuffer;

/** @addtogroup serialization
* @{
*/

/**
 * @class IBitStream
 *
 * 비트 단위의 Input stream.
 */
class IBitStream : public IStream
{
public:
    IBitStream(StreamBuffer& streamBuffer);

    /// 테스트용
    int getReadBitCount() const {
        return bitReadCount_;
    }
public:
    virtual void read(UInt64& value, int bitCount) {
        readBits(value, bitCount);
    }

    virtual void read(UInt32& value, int bitCount) {
        readNumeric(value, bitCount);
    }

    virtual void read(UInt16& value, int bitCount) {
        readNumeric(value, bitCount);
    }

    virtual void read(UInt8& value, int bitCount) {
        readNumeric(value, bitCount);
    }

    virtual void read(Int64& value, int bitCount) {
        readBits(*static_cast<UInt64*>(static_cast<void*>(&value)), bitCount);
        if (bitCount < Bits<Int64>::size) {
            const UInt64 mask = (1 << bitCount) - 1;
            const UInt64 sign = (value & mask) >> (bitCount - 1);
            if (sign != 0) {
                value |= ~mask;
            }
        }
    }

    virtual void read(Int32& value, int bitCount) {
        readSignedNumeric(value, bitCount);
    }

    virtual void read(Int16& value, int bitCount) {
        readSignedNumeric(value, bitCount);
    }

    virtual void read(Int8& value, int bitCount) {
        readSignedNumeric(value, bitCount);
    }

    virtual void read(bool& value, int bitCount) {
        UInt32 numeric;
        readNumeric(numeric, bitCount);
        value = (numeric != 0) ? true : false;
    }

    virtual void read(Float32& value, int bitCount) {
        UInt32 numeric;
        readNumeric(numeric, bitCount);
        value = *static_cast<const Float32*>(static_cast<const void*>(&numeric));
    }

    virtual void read(String& value, size_t maxLength, int sizeBitCount);

    virtual void read(WString& value, size_t maxLength, int sizeBitCount);

    virtual void read(void* buffer, UInt16 length);

    virtual void align();

    virtual void reset(bool resetBuffer);

    virtual int getTotalBitCount() const;

    virtual int getTotalSize() const {
        return toBytes(getTotalBitCount());
    }
private:
    template <typename T>
    void readNumeric(T& value, int bitCount) {
        UInt32 numeric;
        readBits(numeric, bitCount);
        value = static_cast<T>(numeric);
    }
    template <typename T>
    void readSignedNumeric(T& value, int bitCount) {
        UInt32 numeric;
        readBits(numeric, bitCount);
        if (bitCount < Bits<T>::size) {
            const UInt32 mask = (1 << bitCount) - 1;
            const UInt32 sign = (numeric & mask) >> (bitCount - 1);
            if (sign != 0) {
                numeric |= ~mask;
            }
        }
        value = static_cast<T>(static_cast<Int32>(numeric));
    }
    void readBits(UInt32& value, int bitCount);
    void readBits(UInt64& value, int bitCount);
    UInt8 readByte() {
        UInt8 ch;
        read(ch, Bits<UInt8>::size);
        return ch;
    }
private:
    StreamBuffer& streamBuffer_;
    /// 읽혀진 데이터의 비트 수(<= 8)
    int bitReadCount_;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_IBITSTREAM_H
