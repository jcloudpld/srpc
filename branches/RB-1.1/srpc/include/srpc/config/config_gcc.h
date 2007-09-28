#ifndef SRPC_CONFIG_GCC_H
#define SRPC_CONFIG_GCC_H

// GCC Configuration

#if defined(GCC_HASCLASSVISIBILITY) && defined(SRPC_DLL) && defined(SRPC_EXPORTS)
#  define SRPC_API __attribute__ ((visibility("default")))
#  define SRPC_API_INLINE SRPC_API
#else
#  define SRPC_API
#  define SRPC_API_INLINE
#endif

#endif // SRPC_CONFIG_GCC_H
