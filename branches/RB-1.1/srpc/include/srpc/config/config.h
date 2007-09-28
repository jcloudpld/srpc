#ifndef SRPC_CONFIG_H
#define SRPC_CONFIG_H

/// boost::pool_allocator, boost::fast_pool_allocator�� ����� ��� uncomment.
#define USE_BOOST_POOL_ALLOCATOR

#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error ����� MSVC�� GCC�� �����մϴ�
#endif

#endif // SRPC_CONFIG_H
