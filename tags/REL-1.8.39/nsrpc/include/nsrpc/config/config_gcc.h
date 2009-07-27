#ifndef NSRPC_CONFIG_GCC_H
#define NSRPC_CONFIG_GCC_H

// GCC Configuration

#if defined(NSRPC_GCC_HAS_CLASSVISIBILITY) && \
    defined(NSRPC_DLL) && defined(NSRPC_EXPORTS)
#  define NSRPC_API __attribute__ ((visibility("default")))
#else
#  define NSRPC_API
#endif

#endif // NSRPC_CONFIG_GCC_H
