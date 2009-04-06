#ifndef NSRPC_NSRPC_H
#define NSRPC_NSRPC_H

#if defined (_MSC_VER)
#   pragma once
#endif

#include "Version.h"

#include <srpc/srpc.h>

#include <ace/config.h>

// Proactor supported?
#if defined (ACE_HAS_AIO_CALLS) || \
    (defined (ACE_WIN32) && defined (ACE_HAS_WIN32_OVERLAPPED_IO))
#   define NSRPC_HAS_PROACTOR  1
#else
#   undef NSRPC_HAS_PROACTOR
#endif

// use TProactor?
/* #define NSRPC_USE_TPROACTOR  0 */

#define USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER

#if defined (_MSC_VER)
#   include "config/config_msvc.h"
#elif defined (__GNUC__)
#   include "config/config_gcc.h"
#endif

#endif // NSRPC_NSRPC_H
