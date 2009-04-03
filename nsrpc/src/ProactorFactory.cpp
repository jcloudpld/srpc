#include "stdafx.h"
#include <nsrpc/ProactorFactory.h>

#if defined (NSRPC_HAS_PROACTOR)

#include <srpc/srpc_macros.h>

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4250 4267 4355 4800)
#endif
#if defined (NSRPC_USE_TPROACTOR)
#  include <TProactor/Proactor.h>
#  if defined (ACE_WIN32) 
#    include <TProactor/WIN32_Proactor.h>
#  else  /* defined (ACE_WIN32) */
#    include <TProactor/POSIX_Proactor.h>
#    include <TProactor/POSIX_CB_Proactor.h>
#    include <TProactor/SUN_Proactor.h>
#  endif /* defined (ACE_WIN32) */
#else // NSRPC_USE_TPROACTOR
#  if defined (ACE_WIN32) 
#    include <ace/WIN32_Proactor.h>
#  else  /* defined (ACE_WIN32) */
#    include <ace/POSIX_Proactor.h>
#    include <ace/POSIX_CB_Proactor.h>
#    include <ace/SUN_Proactor.h>
#  endif /* defined (ACE_WIN32) */
#endif // NSRPC_USE_TPROACTOR
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace nsrpc
{

ProactorType toProactorType(const srpc::String& typeString)
{
    struct ProactorTable {
        ProactorType type_;
        const char* typeString_;
    };

    const ProactorTable proactorTable[] =
    {
        { ptWin32, "iocp" },
        { ptSelect, "select" },
        { ptPoll, "poll" },
        { ptEpoll, "epoll" },
        { ptLinuxRt, "linuxrt" },
        { ptLinux, "linux" },
        { ptDevPoll, "devpoll" },
        { ptSunPort, "sunport" },
        { ptAioCb, "aiocb" },
        { ptSig, "sig" },
        { ptCallback, "callback" },
        { ptSun, "sun" }
    };

    for (int i = 0; i < ptCount; ++i) {
        if (typeString == proactorTable[i].typeString_) {
            return proactorTable[i].type_;
        }
    }
    return ptUnknown;
}

// = ProactorFactory

NSRPC_Proactor* ProactorFactory::create(ProactorType ptype)
{
    NSRPC_Proactor_Impl* proactorImpl = 0;

# if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

    if (ptype == ptWin32) {
        proactorImpl = new NSRPC_WIN32_Proactor;
    }

# else

    // POSIX : > 0 max number aio operations  proactor,
    const size_t maxAioOperations = 0;
    SRPC_UNUSED_ARG(maxAioOperations);

#  if defined (NSRPC_USE_TPROACTOR)

    // POSIX : signal to interrupt (0- PIPE Strategy,
    //                              signalToInterrupt - SIGNAL Strategy)
    const int  signalToInterrupt = 0;
    // POSIX : leader type :  0-shared / 1-dedicated
    const int  leaderType = 0;
    // POSIX : start aio type :  1 - any thread , 0- only leader
    const int startAioType = 1;

    switch (ptype) {
    case ptSelect:
        proactorImpl = new TRB_Select_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType);
        break;
    case ptPoll:
        proactorImpl = new TRB_Poll_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType);
        break;
    case ptEpoll:
        proactorImpl = new TRB_Event_Poll_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType);
        break;
    case ptLinuxRt:
        proactorImpl = new TRB_Linux_RT_Proactor(maxAioOperations,
            signalToInterrupt, 1, startAioType);
        break;
    case ptLinux:
        proactorImpl = new TRB_LINUX_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType);
        break;
    case ptSig:
        proactorImpl = new TRB_POSIX_SIG_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType);
        break;
    case ptDevPoll:
        proactorImpl = new TRB_Dev_Poll_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, 1); // start only in leader thread
        break;
    case ptSunPort:
        proactorImpl = new TRB_SUN_Port_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType);
        break;
    default:
        break;
    }

# else

    switch (ptype) {
#   if defined (ptAioCb)
    case ptAioCb:
        proactorImpl = new ACE_POSIX_AIOCB_Proactor(maxAioOperations);
        break;
#   endif
#   if defined (ptSun)
    case ptSun:
        proactorImpl = new ACE_SUN_Proactor(maxAioOperations);
        break;
#   endif
#   if defined (ptCallback)
    case ptCallback:
        proactorImpl = new ACE_POSIX_CB_Proactor(maxAioOperations);
        break;
#   endif
#   if defined (ptSig)
    case ptSig:
        proactorImpl = new ACE_POSIX_SIG_Proactor(maxAioOperations);
        break;
#   endif
    default:
        break;
    }

# endif // NSRPC_USE_TPROACTOR

#endif // (ACE_WIN32) && !defined (ACE_HAS_WINCE)

    assert(proactorImpl != 0 && "Unknown Proactor Type");

    return new NSRPC_Proactor(proactorImpl, 1);
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
