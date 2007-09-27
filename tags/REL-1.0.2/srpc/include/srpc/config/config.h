#ifndef SRPC_CONFIG_H
#define SRPC_CONFIG_H

#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error 현재는 MSVC와 GCC만 지원합니다
#endif

#endif // SRPC_CONFIG_H
