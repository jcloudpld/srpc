#include "stdafx.h"
#include <nsrpc/SessionAcceptor.h>
#include <nsrpc/Session.h>
#include <nsrpc/detail/SessionCreator.h>
#include <nsrpc/utility/SystemUtil.h>
#include <nsrpc/utility/Logger.h>

namespace nsrpc
{

bool SessionAcceptor::open(const ACE_INET_Addr& listenAddr,
    NSRPC_Proactor* proactor, size_t numberOfInitialAccepts)
{
    if (shouldFinish_) {
        return false;
    }

    // 리눅스(2.6.x)에서 backlog값을 작게 설정하면 해당 수치 이상의 컨넥션이
    // 동시에 발생할 경우 약 2~3초 정도의 블럭킹이 발생하게 된다.
    const int backlog = (std::max)(ACE_DEFAULT_BACKLOG, SOMAXCONN);

    const int pass_addresses = 1;
    const int reuse_addr = 1;
    const int validate_new_connection = 1;
    const int reissue_accept = 0;
    if (Parent::open(listenAddr, 0, pass_addresses, backlog, reuse_addr,
        proactor, validate_new_connection, reissue_accept, 0) != 0) {
        NSRPC_LOG_ERROR(ACE_TEXT("SessionAcceptor::start() - ")
            ACE_TEXT("NSRPC_Asynch_Acceptor::open() FAILED(%m)."));
        return false;
    }

    for (size_t i = 0; i < numberOfInitialAccepts; i++) {
        if (this->accept() == -1) {
            NSRPC_LOG_ERROR(ACE_TEXT("SessionAcceptor::start() - ")
                ACE_TEXT("NSRPC_Asynch_Acceptor::accept() FAILED(%m)."));
            return false;
        }
#ifdef USE_TPROACTOR
        ++pendingCount_;
#endif
    }

    return true;
}


void SessionAcceptor::start()
{
    started_ = true;
}


void SessionAcceptor::close()
{
    started_ = false;
    shouldFinish_ = true;

    (void)cancel();

    ACE_OS::closesocket(handle());
    handle(ACE_INVALID_HANDLE);
}


void SessionAcceptor::wait()
{
    while (! isSafeToDelete()) {
        pause(1);
    }
}


NSRPC_Service_Handler* SessionAcceptor::make_handler()
{
    return sessionCreator_.acquire();
}


int SessionAcceptor::validate_connection(
    const NSRPC_Asynch_Accept::Result& result,
    const ACE_INET_Addr& remote, const ACE_INET_Addr& local)
{
    ACE_UNUSED_ARG(local);

    if (! started_) {
        return -1;
    }

#ifdef USE_TPROACTOR
    assert(pendingCount_ > 0);
    --pendingCount_;
#endif

    int rc = -1;
    if (! shouldFinish_) {
        if (result.success() != 0) {
            ACE_TCHAR address[MAXHOSTNAMELEN];
            NSRPC_LOG_DEBUG3(ACE_TEXT("Connecting from %s:%d..."),
                remote.get_host_addr(address, MAXHOSTNAMELEN),
                remote.get_port_number());
            rc = 0;
        }
    }
    return rc;
}


int SessionAcceptor::should_reissue_accept()
{
    if (! shouldFinish_) {
        if (accept() == -1) {
            NSRPC_LOG_ERROR(
                ACE_TEXT("SessionAcceptor::should_reissue_accept() - ")
                ACE_TEXT("NSRPC_Asynch_Acceptor::accept() FAILED(%m)."));
        }
        else {
#ifdef USE_TPROACTOR
            ++pendingCount_;
#endif
        }
    }

    return 0;
}

} // namespace nsrpc
