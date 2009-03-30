#ifndef SRPC_SRPC_MACROS_H
#define SRPC_SRPC_MACROS_H

#if defined(_MSC_VER)
#pragma once
#endif

// stolen from ACE
#if defined (ghs) || defined (__GNUC__) || defined (__hpux) || defined (__sgi) || defined (__DECCXX) || defined (__rational__) || defined (__USLC__) || defined (SRPC_RM544) || defined (__DCC__) || defined (__PGI) || defined (__TANDEM)
// Some compilers complain about "statement with no effect" with (a).
// This eliminates the warnings, and no code is generated for the null
// conditional statement.  @note that may only be true if -O is enabled,
// such as with GreenHills (ghs) 1.8.8.
#  define SRPC_UNUSED_ARG(a) do {/* null */} while (&a == 0)

#elif defined (__DMC__)

#  define SRPC_UNUSED_ID(identifier)
template <class T>
inline void SRPC_UNUSED_ARG(const T& SRPC_UNUSED_ID(t)) { }

#else /* ghs || __GNUC__ || ..... */
#  define SRPC_UNUSED_ARG(a) (a)
#endif /* ghs || __GNUC__ || ..... */

#if defined (_MSC_VER) || defined(__sgi) || defined (ghs) || defined (__DECCXX) || defined(__BORLANDC__) || defined (SRPC_RM544) || defined (__USLC__) || defined (__DCC__) || defined (__PGI) || defined (__TANDEM) || (defined (__HP_aCC) && (__HP_aCC >= 60500))
#  define SRPC_NOTREACHED(a)
#else  /* __sgi || ghs || ..... */
#  define SRPC_NOTREACHED(a) a
#endif /* __sgi || ghs || ..... */

#endif // SRPC_SRPC_MACROS_H
