#ifndef NSRPC_CONFIG_H
#define NSRPC_CONFIG_H

#include <srpc/config/config.h>

#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error ����� MSVC�� GCC�� �����մϴ�
#endif

#endif // NSRPC_CONFIG_H
