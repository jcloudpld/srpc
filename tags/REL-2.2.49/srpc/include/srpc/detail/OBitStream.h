#ifndef SRPC_OBITSTREAM_H
#define SRPC_OBITSTREAM_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../OStream.h"

namespace srpc {

class StreamBuffer;

/** @addtogroup serialization
* @{
*/

/**
 * @class OBitStream
 *
 * 비트 단위의 Output stream.
 */
class OBitStream : public OStream
{
public:
    OBitStream(StreamBuffer& streamBuffer, bool shouldUseUtf8ForString);

    /// 테스트용
    int getHoldingBitCount() const {
        return holdingBitCount_;
    }
private:
    virtual void write(UInt64 value, int bitCount) {
        writeBits(value, bitCount);
    }

    virtual void write(UInt32 value, int bitCount) {
        writeBits(value, bitCount);
    }

    virtual void write(UInt16 value, int bitCount) {
        writeBits(UInt32(value), bitCount);
    }

    virtual void write(UInt8 value, int bitCount) {
        writeBits(UInt32(value), bitCount);
    }

    virtual void write(bool value, int bitCount) {
        writeBits(UInt32(value), bitCount);
    }

    virtual void write(Int64 value, int bitCount) {
        writeBits(UInt64(value), bitCount);
    }

    virtual void write(Int32 value, int bitCount) {
        writeBits(UInt32(value), bitCount);
    }

    virtual void write(Int16 value, int bitCount) {
        writeBits(UInt32(Int32(value)), bitCount);
    }

    virtual void write(Int8 value, int bitCount) {
        writeBits(UInt32(Int32(value)), bitCount);
    }

    virtual void write(Float32 value, int /*bitCount*/) {
        writeBits(*static_cast<UInt32*>(static_cast<void*>(&value)),
            Bits<Float32>::size);
    }

    virtual void write(const String& value, size_t maxLength,
        int sizeBitCount);

    virtual void write(const WString& value, size_t maxLength,
        int sizeBitCount);

    virtual void write(const void* buffer, UInt16 length);

    virtual void align();
    virtual void reset(bool resetBuffer);

    virtual int getTotalBitCount() const {
        return totalBitCount_;
    }

    virtual int getTotalSize() const;
private:
    /// 비트 스트림을 스트림 버퍼에 기록한다
    void writeBits(UInt32 value, int bitCount);
    void writeBits(UInt64 value, int bitCount);

    void writeByte(UInt8 value) {
        write(value, Bits<UInt8>::size);
    }
private:
    StreamBuffer& streamBuffer_;
    const bool shouldUseUtf8ForString_;
    /// 저장할 비트 수
    int holdingBitCount_;
    /// 저장된 비트 수
    int totalBitCount_;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_OBITSTREAM_H
