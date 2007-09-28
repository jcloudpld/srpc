#ifndef NSRPC_CONFIG_H
#define NSRPC_CONFIG_H

#include <srpc/config/config.h>

#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error 현재는 MSVC와 GCC만 지원합니다
#endif

#endif // NSRPC_CONFIG_H
