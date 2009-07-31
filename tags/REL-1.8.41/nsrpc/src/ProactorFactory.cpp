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
        { ptAny, "any" },
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


bool isSupportedProactor(ProactorType proactorType)
{
    if (ptAny == proactorType) {
        return true;
    }

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

// = ProactorFactory

std::auto_ptr<NSRPC_Proactor> ProactorFactory::create(ProactorType proactorType)
{
    std::auto_ptr<NSRPC_Proactor_Impl> proactorImpl;

# if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

    if (proactorType == ptWin32) {
        proactorImpl.reset(new NSRPC_WIN32_Proactor);
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

    switch (proactorType) {
    case ptSelect:
        proactorImpl.reset(new TRB_Select_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType));
        break;
    case ptPoll:
        proactorImpl.reset(new TRB_Poll_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType));
        break;
    case ptEpoll:
        proactorImpl.reset(new TRB_Event_Poll_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType));
        break;
    case ptLinuxRt:
        proactorImpl.reset(new TRB_Linux_RT_Proactor(maxAioOperations,
            signalToInterrupt, 1, startAioType));
        break;
    case ptLinux:
        proactorImpl.reset(new TRB_LINUX_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType));
        break;
    case ptSig:
        proactorImpl.reset(new TRB_POSIX_SIG_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType));
        break;
    case ptDevPoll:
        proactorImpl.reset(new TRB_Dev_Poll_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, 1)); // start only in leader thread
        break;
    case ptSunPort:
        proactorImpl.reset(new TRB_SUN_Port_Proactor(maxAioOperations,
            signalToInterrupt, leaderType, startAioType));
        break;
    default:
        break;
    }

# else

#   if defined (ACE_POSIX_AIOCB_PROACTOR)
    if (ptAioCb == proactorType) {
        proactorImpl.reset(new ACE_POSIX_AIOCB_Proactor(maxAioOperations));
    }
#   endif

#   if defined (ACE_POSIX_SIG_PROACTOR) || defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
    if (ptSig == proactorType) {
        proactorImpl.reset(new ACE_POSIX_SIG_Proactor(maxAioOperations));
    }
#   endif

#   if !defined(ACE_HAS_BROKEN_SIGEVENT_STRUCT)
    if (ptCallback == proactorType) {
        proactorImpl.reset(new ACE_POSIX_CB_Proactor(maxAioOperations));
    }
#   endif

#   if defined (sun)
    if (ptSun == proactorType) {
        proactorImpl.reset(new ACE_SUN_Proactor(maxAioOperations));
    }
#   endif

# endif // NSRPC_USE_TPROACTOR

#endif // (ACE_WIN32) && !defined (ACE_HAS_WINCE)

    if ((! proactorImpl.get()) && (ptAny != proactorType)) {
        assert(false && "Unknown Proactor Type");
    }

    return std::auto_ptr<NSRPC_Proactor>(
        new NSRPC_Proactor(proactorImpl.release(), 1));
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
