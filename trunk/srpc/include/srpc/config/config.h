#ifndef SRPC_CONFIG_H
#define SRPC_CONFIG_H

/// boost::pool_allocator를 사용할 경우 uncomment.
//#define USE_BOOST_POOL_ALLOCATOR_FOR_SRPC

/// boost::fast_pool_allocator를 사용할 경우 uncomment.
#define USE_BOOST_FAST_POOL_ALLOCATOR_FOR_SRPC

/// string에 boost::pool_allocator를 사용할 경우 uncomment.
//#define USE_BOOST_POOL_ALLOCATOR_FOR_SRPC_STRING


#if defined (_MSC_VER)
#   include "config_msvc.h"
#elif defined(__GNUC__)
#   include "config_gcc.h"
#else
#   error 현재는 MSVC와 GCC만 지원합니다
#endif

#endif // SRPC_CONFIG_H
