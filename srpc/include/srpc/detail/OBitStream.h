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
 * ��Ʈ ������ Output stream.
 */
class OBitStream : public OStream
{
public:
    OBitStream(StreamBuffer& streamBuffer, bool shouldUseUtf8ForString);

    /// �׽�Ʈ��
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
    /// ��Ʈ ��Ʈ���� ��Ʈ�� ���ۿ� ����Ѵ�
    void writeBits(UInt32 value, int bitCount);
    void writeBits(UInt64 value, int bitCount);

    void writeByte(UInt8 value) {
        write(value, Bits<UInt8>::size);
    }
private:
    StreamBuffer& streamBuffer_;
    const bool shouldUseUtf8ForString_;
    /// ������ ��Ʈ ��
    int holdingBitCount_;
    /// ����� ��Ʈ ��
    int totalBitCount_;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_OBITSTREAM_H
