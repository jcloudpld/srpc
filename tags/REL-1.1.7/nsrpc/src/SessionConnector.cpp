#include "stdafx.h"
#include <nsrpc/SessionConnector.h>
#include <nsrpc/Session.h>
#include <nsrpc/detail/SessionCreator.h>
#include <nsrpc/utility/SystemUtil.h>
#include <nsrpc/utility/Logger.h>

namespace nsrpc
{

size_t SessionConnector::start(const ACE_INET_Addr& serverAddr,
    NSRPC_Proactor* proactor, size_t numOfConnection)
{
    if (shouldFinish_) {
        return false;
    }

    const int passAddresses = 1;
    const int validateNewConnection = 1;
    if (open(passAddresses, proactor, validateNewConnection) != 0) {
        NSRPC_LOG_ERROR(ACE_TEXT("SessionConnector::start() - ")
            ACE_TEXT("NSRPC_Asynch_Connector::open() FAILED(%m)."));
        return false;
    }

    size_t connectionCount = 0;
    for (; connectionCount < numOfConnection; ++connectionCount) {
        int rc = Parent::connect(serverAddr);
        if (rc != 0) {
            NSRPC_LOG_ERROR(ACE_TEXT("SessionConnector::start() - ")
                ACE_TEXT("NSRPC_Asynch_Connector::connect() FAILED(%m)."));
            break;
        }
#ifdef USE_TPROACTOR
        ++pendingCount_;
#endif
    }

    return connectionCount;
}


void SessionConnector::stop()
{
    shouldFinish_ = true;

    (void)Parent::cancel();
}


void SessionConnector::wait()
{
    while (! isSafeToDelete()) {
        pause(1);
    }
}


NSRPC_Service_Handler* SessionConnector::make_handler()
{
    return sessionCreator_.acquire();
}


int SessionConnector::validate_connection(
    const NSRPC_Asynch_Connect::Result& result,
    const ACE_INET_Addr& remote, const ACE_INET_Addr& local)
{
    ACE_UNUSED_ARG(local);

#ifdef USE_TPROACTOR
    --pendingCount_;
#endif

    int rc = -1;
    if (! shouldFinish_) {
        if (result.success() != 0) {
            ACE_TCHAR address[MAXHOSTNAMELEN];
            NSRPC_LOG_DEBUG3(ACE_TEXT("Connecting to %s:%d..."),
                remote.get_host_addr(address, MAXHOSTNAMELEN),
                remote.get_port_number());
            rc = 0;
        }
    }
    return rc;
}

} // namespace nsrpc
