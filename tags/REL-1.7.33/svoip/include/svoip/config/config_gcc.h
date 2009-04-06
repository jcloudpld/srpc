#ifndef SVOIP_CONFIG_GCC_H
#define SVOIP_CONFIG_GCC_H

// GCC Configuration

#if defined(GCC_HASCLASSVISIBILITY) && defined(SVOIP_DLL) && defined(SVOIP_EXPORTS)
#  define SVOIP_API __attribute__ ((visibility("default")))
#  define SVOIP_API_INLINE SVOIP_API
#else
#  define SVOIP_API
#  define SVOIP_API_INLINE
#endif

#endif // SVOIP_CONFIG_GCC_H
