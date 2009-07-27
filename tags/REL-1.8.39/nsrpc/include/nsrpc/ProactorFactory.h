#ifndef NSRPC_PROACTORFACTORY_H
#define NSRPC_PROACTORFACTORY_H

#include "nsrpc.h"
#include "config/Proactor.h"

#if defined (NSRPC_HAS_PROACTOR)

#include <srpc/StringTypes.h>
#include <memory>

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

    ptAny, ///< depends on ACE

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

bool NSRPC_API isSupportedProactor(ProactorType proactorType);

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
    static std::auto_ptr<NSRPC_Proactor> create(ProactorType proactorType);
};

/** @} */ // addtogroup proactor

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // NSRPC_PROACTORFACTORY_H
