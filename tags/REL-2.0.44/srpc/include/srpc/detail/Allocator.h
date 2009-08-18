#ifndef SRPC_ALLOCATOR_H
#define SRPC_ALLOCATOR_H

#ifdef _MSC_VER
#  pragma once
#endif

#if defined(USE_BOOST_POOL_ALLOCATOR_FOR_SRPC) || \
    defined(USE_BOOST_FAST_POOL_ALLOCATOR_FOR_SRPC) || \
    defined(USE_BOOST_POOL_ALLOCATOR_FOR_SRPC_STRING)

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127)
#endif
#include <boost/pool/pool_alloc.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#else

#include <memory>

#endif

#ifdef USE_BOOST_POOL_ALLOCATOR_FOR_SRPC
#  define SrpcNormalAllocator boost::pool_allocator
#else
#  define SrpcNormalAllocator std::allocator
#endif

#ifdef USE_BOOST_FAST_POOL_ALLOCATOR_FOR_SRPC
#  define SrpcNodeAllocator boost::fast_pool_allocator
#else
#  define SrpcNodeAllocator std::allocator
#endif

#ifdef USE_BOOST_POOL_ALLOCATOR_FOR_SRPC_STRING
#  define SrpcStringAllocator boost::pool_allocator
#else
#  define SrpcStringAllocator std::allocator
#endif

#endif // SRPC_ALLOCATOR_H
