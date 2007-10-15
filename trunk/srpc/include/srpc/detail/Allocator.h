#ifndef SRPC_ALLOCATOR_H
#define SRPC_ALLOCATOR_H

#include "../config/config.h"

#ifdef USE_BOOST_POOL_ALLOCATOR_FOR_SRPC

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127)
#endif
#include <boost/pool/pool_alloc.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#define SrpcNormalAllocator boost::pool_allocator
#define SrpcNodeAllocator boost::fast_pool_allocator

#else // ! USE_BOOST_POOL_ALLOCATOR_FOR_SRPC

#include <memory>

#define SrpcNormalAllocator std::allocator
#define SrpcNodeAllocator std::allocator

#endif // USE_BOOST_POOL_ALLOCATOR_FOR_SRPC

#endif // SRPC_ALLOCATOR_H
