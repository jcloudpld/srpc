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
    ptAioCb, ///< aiosuspend() (POSIX)
    ptCallback, ///< style aio callbacks (SGI)
    ptSun, ///< aiowait() (SunOS)
#ifdef USE_TPROACTOR
    ptSelect, ///< select() (all POSIX systems)
    ptPoll, ///< poll() (all POSIX systems)
    ptEpoll, ///< epoll_wait() (Linux kernel 2.6+)
    ptLinuxRt, ///< waitsiginfo() (Linux kernel 2.4+)
    ptLinux, ///< io_getevents() (Linux kernel 2.6+)
    ptSig, ///< sigwaitinfo() (POSIX)
    ptDevPoll, ///< /dev/poll (SunOS 5.8+)
    ptSunPort, ///< port_get() (SunOS 5.10)
#endif // USE_TPROACTOR

    ptCount
};


/// 문자열을 ProactorType으로 변환
ProactorType NSRPC_API toProactorType(const srpc::String& pt);


/**
* @struct ProactorFactory
*
* NSRPC_Proactor Factory
*/
struct NSRPC_API ProactorFactory
{
    /**
     * Proactor를 생성한다.
     * @param ptype Proactor Type.
     * @return Proactor 객체. 사용 후 반드시 메모리를 해제해야 한다.
     */
    static NSRPC_Proactor* create(ProactorType ptype);
};

/** @} */ // addtogroup proactor

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // NSRPC_PROACTORFACTORY_H
