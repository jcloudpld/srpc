#ifndef NSRPC_CONFIG_H
#define NSRPC_CONFIG_H

#include <srpc/srpc.h>

#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error other compiler(except GCC, VC++) is not supported
#endif

#define USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER

#endif // NSRPC_CONFIG_H
