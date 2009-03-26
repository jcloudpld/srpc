#ifndef SRPC_SRPC_H
#define SRPC_SRPC_H

#if defined(_MSC_VER)
#pragma once
#endif

#if defined(HAVE_CONFIG_H)
#   include "config.h"
#endif

#if defined(_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error other compiler(except GCC, VC++) is not supported
#endif

#endif // SRPC_SRPC_H

