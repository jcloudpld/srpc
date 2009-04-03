#ifndef NSRPC_PROACTORFACTORY_H
#define NSRPC_PROACTORFACTORY_H

#include "nsrpc.h"

#if defined (NSRPC_HAS_PROACTOR)

#include "config/Proactor.h"
#include <srpc/StringTypes.h>

// forward decl.
class NSRPC_Proactor;

namespace nsrpc
{

/** @addtogroup proactor
* @{
*/

/// NSRPC_Proactor Type
enum ProactorType
{
    ptUnknown = -1,

    ptWin32, ///< IOCP (WIN32)

    ptSelect, ///< select() (all POSIX systems)

    ptPoll, ///< poll() (all POSIX systems)

    ptEpoll, ///< epoll_wait() (Linux kernel 2.6+)
    ptLinux, ///< io_getevents() (Linux kernel 2.6+)
    ptLinuxRt, ///< waitsiginfo() (Linux kernel 2.4+)

    ptSig, ///< sigwaitinfo() (POSIX)

    ptDevPoll, ///< /dev/poll (SunOS 5.8+)

    ptSunPort, ///< port_get() (SunOS 5.10)

    ptAioCb, ///< aiosuspend() (POSIX)

    ptCallback, ///< aio callbacks (SGI)

    ptSun, ///< aiowait() (SunOS)

    ptCount
};


/// convert a string to ProactorType
ProactorType NSRPC_API toProactorType(const srpc::String& pt);


inline bool isSupportedProactor(ProactorType proactorType)
{
# if defined (ACE_WIN32)

    if (ptWin32 == proactorType) {
        return true;
    }

# elif defined (NSRPC_USE_TPROACTOR)

    if (ptSelect == proactorType) {
        return true;
    }

#   if defined (ACE_HAS_POLL)
    if (ptPoll == proactorType) {
        return true;
    }
#   endif

#   if defined (linux)
#     if defined (ACE_HAS_EVENT_POLL)
    if (ptEpoll == proactorType) {
        return true;
    }
#     endif
#     if defined (ACE_HAS_LINUX_LIBAIO)
    if (ptLinux == proactorType) {
        return true;
    }
#     endif
    if (ptLinuxRt == proactorType) {
        return true;
    }
#   endif

#   if defined (ACE_POSIX_SIG_PROACTOR) || defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
    if (ptSig == proactorType) {
        return true;
    }
#   endif

#   if defined (ACE_HAS_DEV_POLL)
    if (ptDevPoll == proactorType) {
        return true;
    }
#   endif

#   if defined (sun)
    if (ptSunPort == proactorType) {
        return true;
    }
#   endif

# else

#   if defined (ACE_POSIX_AIOCB_PROACTOR)
    if (ptAioCb == proactorType) {
        return true;
    }
#   endif

#   if defined (ACE_POSIX_SIG_PROACTOR) || defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
    if (ptSig == proactorType) {
        return true;
    }
#   endif

#   if !defined(ACE_HAS_BROKEN_SIGEVENT_STRUCT)
    if (ptCallback == proactorType) {
        return true;
    }
#   endif

#   if defined (sun)
    if (ptSun == proactorType) {
        return true;
    }
#   endif

# endif

    return false;
}

/**
* @struct ProactorFactory
*
* NSRPC_Proactor Factory
*/
struct NSRPC_API ProactorFactory
{
    /**
     * create a Proactor instance
     * @param proactorType Proactor Type.
     * @return Proactor instance. you must deallocate memory.
     */
    static NSRPC_Proactor* create(ProactorType proactorType);
};

/** @} */ // addtogroup proactor

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // NSRPC_PROACTORFACTORY_H
