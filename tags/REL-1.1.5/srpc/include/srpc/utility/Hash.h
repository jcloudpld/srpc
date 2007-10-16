#ifndef SRPC_HASH_H
#define SRPC_HASH_H

#include "../config/config.h"
#include "../Types.h"

namespace srpc {

/// hash string
UInt32 SRPC_API hash(const char* str, size_t len);

} // namespace srpc

#endif // SRPC_HASH_H
