#ifndef NSRPC_PROTOCOL_H
#define NSRPC_PROTOCOL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"
#include <srpc/Types.h>
#include <srpc/utility/Endian.h>

namespace nsrpc
{

/** @addtogroup protocol
* @{
*/

namespace packet
{

/**
 * 버퍼에서 기본 타입(primitive type)의 데이터를 읽어온다
 */
template <typename T>
inline size_t get(T& value, const void* buffer) {
    value = srpc::toRpcByteOrder(*static_cast<const T*>(buffer));
    return sizeof(T);
}

/**
 * 버퍼에 기본 타입(primitive type)의 데이터를 저장한다
 */
template <typename T>
inline size_t put(T value, void* buffer) {
    *static_cast<T*>(buffer) = srpc::toRpcByteOrder(value);
    return sizeof(T);
}

} // namespace packet

/// Packet body size
typedef srpc::UInt16 BodySize;

/**
 * @struct PacketHeader
 * 패킷 헤더
 * - packet := <header> + <body>
 * - header := <body-size>
 * - body-size := sizeof(body)
 */
struct NSRPC_API AbstractPacketHeader
{
    BodySize bodySize_;

    AbstractPacketHeader() :
        bodySize_(0) {}

    virtual ~AbstractPacketHeader() {}

    static size_t getHeaderSize() {
        return sizeof(BodySize);
    }

    virtual void reset() {
        bodySize_ = 0;
    }

    virtual bool isValid() const {
        return bodySize_ > 0;
    }

    virtual size_t write(void* buffer) const {
        assert(bodySize_ > 0);
        return packet::put(bodySize_, buffer);
    }

    virtual size_t read(const void* buffer) {
        return packet::get(bodySize_, buffer);
    }
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // !defined(NSRPC_PROTOCOL_H)
