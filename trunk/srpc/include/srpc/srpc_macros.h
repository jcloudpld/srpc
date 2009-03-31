#ifndef SRPC_SRPC_MACROS_H
#define SRPC_SRPC_MACROS_H

#if defined(_MSC_VER)
#pragma once
#endif

// stolen from boost/statechart/detail/avoid_unused_warning.hpp
template< typename T >
inline void avoid_unused_warning(const T &) {}

#define SRPC_UNUSED_ARG     avoid_unused_warning

#endif // SRPC_SRPC_MACROS_H
