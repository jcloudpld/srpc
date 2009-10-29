#ifndef SRPC_HASH_H
#define SRPC_HASH_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../Types.h"

namespace srpc {

/// hash string
/// - PJW hash or CRC variant
/// - http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
inline UInt32 hash(const char* str, size_t len)
{
    UInt32 h = 0;

#ifdef USE_PJW_HASH
    // PJW hash
    for (size_t i = 0; i < len; ++i) {
        const char ki = str[i];
        h = (h << 4) + (ki * 13);

        const UInt32 g = hash & 0xf0000000;
        if (g != 0) {
            h ^= (g >> 24);
            h ^= g;
        }
    }
#else
    // CRC variant
    for (size_t i = 0; i < len; ++i) {
        const char ki = str[i];
        const UInt32 highorder = h & 0xf8000000;
        h <<= 5;
        h ^= (highorder >> 27);
        h ^= ki;
    }
#endif
    return h;
}

} // namespace srpc

#endif // SRPC_HASH_H
