#include "stdafx.h"

#if defined (NSRPC_HAS_PROACTOR)

#include <nsrpc/ProactorSessionAcceptor.h>
#include <nsrpc/ProactorSession.h>
#include <nsrpc/SessionValidator.h>
#include <nsrpc/detail/SessionCreator.h>
#include <nsrpc/utility/SystemUtil.h>
#include <nsrpc/utility/Logger.h>

namespace nsrpc
{

bool ProactorSessionAcceptor::open(const ACE_INET_Addr& listenAddr,
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
    const int reissue_accept = 1;
    if (Parent::open(listenAddr, 0, pass_addresses, backlog, reuse_addr,
        proactor, validate_new_connection, reissue_accept, 0) != 0) {
        NSRPC_LOG_ERROR(ACE_TEXT("ProactorSessionAcceptor::start() - ")
            ACE_TEXT("NSRPC_Asynch_Acceptor::open() FAILED(%m)."));
        return false;
    }

    for (size_t i = 0; i < numberOfInitialAccepts; i++) {
        if (accept(0, 0) == -1) {
            NSRPC_LOG_ERROR(ACE_TEXT("ProactorSessionAcceptor::start() - ")
                ACE_TEXT("NSRPC_Asynch_Acceptor::accept() FAILED(%m)."));
            return false;
        }
    }

    return true;
}


void ProactorSessionAcceptor::close()
{
    shouldFinish_ = true;

    (void)cancel();

    ACE_OS::closesocket(handle());
    handle(ACE_INVALID_HANDLE);
}


void ProactorSessionAcceptor::wait()
{
    while (! isSafeToDelete()) {
        pause(1);
    }
}


int ProactorSessionAcceptor::accept(size_t bytes_to_read, const void *act)
{
    const int result = Parent::accept(bytes_to_read, act);
    if (result == -1) {
        return -1;
    }

#if defined (NSRPC_USE_TPROACTOR)
    ++pendingCount_;
#endif

    return result;
}


NSRPC_Service_Handler* ProactorSessionAcceptor::make_handler()
{
    return static_cast<ProactorSession*>(sessionCreator_.acquire());
}


int ProactorSessionAcceptor::validate_connection(
    const NSRPC_Asynch_Accept::Result& result,
    const ACE_INET_Addr& remote, const ACE_INET_Addr& local)
{
    ACE_UNUSED_ARG(local);

#if defined (NSRPC_USE_TPROACTOR)
    assert(pendingCount_ > 0);
    --pendingCount_;
#endif

    if (shouldFinish_) {
        return -1;
    }

    ACE_TCHAR address[MAXHOSTNAMELEN];
    if ((validator_ != 0) && (! validator_->isValidated())) {
        NSRPC_LOG_DEBUG3(ACE_TEXT("Blocked connection from %s:%d."),
            remote.get_host_addr(address, MAXHOSTNAMELEN),
            remote.get_port_number());
        return -1;
    }

    if (! result.success()) {
        return -1;
    }

    NSRPC_LOG_DEBUG3(ACE_TEXT("Connecting from %s:%d..."),
        remote.get_host_addr(address, MAXHOSTNAMELEN),
        remote.get_port_number());
    return 0;
}


int ProactorSessionAcceptor::should_reissue_accept()
{
    if (shouldFinish_) {
        return 0;
    }

    return Parent::should_reissue_accept();
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
