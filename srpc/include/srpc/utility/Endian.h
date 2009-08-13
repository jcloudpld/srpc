#ifndef SRPC_ENDIAN_H
#define SRPC_ENDIAN_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../Types.h"
#include <boost/detail/endian.hpp>
#include <cassert>

// TODO: big-endian system에서 테스트

namespace srpc {

namespace {

// primary template
template <typename T, std::size_t s = sizeof(T)>
struct RpcByteOrderImpl;

// 1 byte
template <typename T>
struct RpcByteOrderImpl<T, 1>
{
    static T convert(T value) {
        return value;
    }
};

// 2 bytes
template <typename T>
struct RpcByteOrderImpl<T, 2>
{
    static T convert(T value) {
        UInt8* src = static_cast<UInt8*>(static_cast<void*>(&value));
        UInt8* dest = src + sizeof(T) - 1;
        *dest-- = *src++;
        *dest = *src;
        return value;
    }
};

// 4 bytes
template <typename T>
struct RpcByteOrderImpl<T, 4>
{
    static T convert(T value) {
        UInt8* src = static_cast<UInt8*>(static_cast<void*>(&value));
        UInt8* dest = src + sizeof(T) - 1;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest = *src;
        return value;
    }
};

// 8 bytes
template <typename T>
struct RpcByteOrderImpl<T, 8>
{
    static T convert(T value) {
        UInt8* src = static_cast<UInt8*>(static_cast<void*>(&value));
        UInt8* dest = src + sizeof(T) - 1;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest-- = *src++;
        *dest = *src;
        return value;
    }
};

} // namespace

/**
 * convert to RPC byte-order(little-endian)
 */
template <typename T>
inline T toRpcByteOrder(T value)
{
#if defined(BOOST_BIG_ENDIAN)
    return RpcByteOrderImpl<T>::convert(value);
#elif !defined(BOOST_LITTLE_ENDIAN)
    assert(false && "unknown byte-order");
#endif
    return value;
}

} // namespace srpc

#endif // SRPC_ENDIAN_H
