#ifndef NSRPC_NSRPC_H
#define NSRPC_NSRPC_H

#if defined (_MSC_VER)
#   pragma once
#endif

#include "Version.h"
#include <srpc/srpc.h>

#define USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER

#if defined (_MSC_VER)
#   include "config/config_msvc.h"
#elif defined (__GNUC__)
#   include "config/config_gcc.h"
#endif

#endif // NSRPC_NSRPC_H
