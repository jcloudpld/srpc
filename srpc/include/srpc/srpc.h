#ifndef SRPC_SRPC_H
#define SRPC_SRPC_H

#if defined (_MSC_VER)
#   pragma once
#endif

#include "Version.h"

#if defined (_MSC_VER)
#   include "srpc/config/config_msvc.h"
#elif defined (__GNUC__)
#   include "srpc/config/config_gcc.h"
#endif

#endif // SRPC_SRPC_H
