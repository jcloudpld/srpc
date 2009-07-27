#include "stdafx.h"
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
#include <ace/Proactor.h>
#include <ace/Init_ACE.h>
#include <cassert>

namespace nsrpc
{

InitAce::InitAce()
{
    ACE::init();
}

InitAce::~InitAce()
{
    ACE::fini();
}


void disableSignals(int sigmin, int sigmax)
{
    assert(sigmin <= sigmax);

#ifndef ACE_WIN32
    sigset_t signalSet;
    if (sigemptyset(&signalSet) == - 1) {
        ACE_ERROR((LM_ERROR, ACE_TEXT("Error: (%P|%t):%m"),
            ACE_TEXT("sigemptyset failed")));
    }

    for (int signal = sigmin; signal <= sigmax; ++signal) {
        sigaddset(&signalSet, signal);
    }

    //  Put the <signalSet>.
    if (ACE_OS::pthread_sigmask(SIG_BLOCK, &signalSet, 0) != 0) {
        ACE_ERROR((LM_ERROR, ACE_TEXT("Error: (%P|%t):%m"),
            ACE_TEXT("pthread_sigmask failed")));
    }
#else
    ACE_UNUSED_ARG(sigmin);
    ACE_UNUSED_ARG(sigmax);
#endif /* ACE_WIN32 */
}


bool setMaximumSocketBufferSize(ACE_HANDLE handle)
{
    int bufsize = ACE_DEFAULT_MAX_SOCKET_BUFSIZ;
    return (ACE_OS::setsockopt(handle, SOL_SOCKET, SO_SNDBUF,
            reinterpret_cast<const char*>(&bufsize), sizeof(bufsize)) != -1) &&
        (ACE_OS::setsockopt(handle, SOL_SOCKET, SO_RCVBUF,
            reinterpret_cast<const char*>(&bufsize), sizeof(bufsize)) != -1);
}


long setupTimer(NSRPC_Proactor& proactor,
    NSRPC_Handler& handler, size_t msec, const void* act)
{
#if defined (NSRPC_HAS_PROACTOR)
    // This only works on Win32 platforms and on Unix platforms supporting
    // POSIX aio calls.

    const long timerId =
        proactor.schedule_timer(handler, act, makeTimeValue(msec));
    assert(timerId != -1);
    return timerId;

#else
    assert(false && "Proactor timer is not supported.");
    return -1;
#endif
}


long setupRepeatingTimer(NSRPC_Proactor& proactor,
    NSRPC_Handler& handler, size_t msec, const void* act)
{
#if defined (NSRPC_HAS_PROACTOR)
    // This only works on Win32 platforms and on Unix platforms supporting
    // POSIX aio calls.

    const long timerId =
        proactor.schedule_repeating_timer(handler, act, makeTimeValue(msec));
    assert(timerId != -1);
    return timerId;

#else
    assert(false && "Proactor timer is not supported.");
    return -1;
#endif
}


void cancelTimer(NSRPC_Proactor& proactor, long& timerId)
{
#if defined (NSRPC_HAS_PROACTOR)
    // This only works on Win32 platforms and on Unix platforms supporting
    // POSIX aio calls.

    if (timerId != -1) {
        proactor.cancel_timer(timerId);
        timerId = -1;
    }

#else
    assert(false && "Proactor timer is not supported.");
#endif
}


InetAddresses getLocalAddresses(u_short port)
{
    InetAddresses addresses;

    ACE_TCHAR hostname[MAXHOSTNAMELEN + 1];
    if (ACE_OS::hostname(hostname, MAXHOSTNAMELEN + 1) != -1) {
        hostent* hp = ACE_OS::gethostbyname(ACE_TEXT_ALWAYS_CHAR(hostname));
        if (hp != 0) {
            int size = 0;
            while (hp->h_addr_list[size] != 0) {
                ++size;
            }
            addresses.reserve(size);

            for (int i = 0; i < size; ++i) {
                const void* vAddr = static_cast<void*>(hp->h_addr_list[i]);
                const in_addr& addr = *static_cast<const in_addr*>(vAddr);
                addresses.push_back(ACE_INET_Addr(port, ntohl(addr.s_addr)));
            }
        }
    }
    return addresses;
}


bool isPrivateAddress(const ACE_INET_Addr& address)
{
    struct AddressRange {
        ACE_UINT32 from_;
        ACE_UINT32 to_;

        AddressRange(const char* ipFrom, const char* ipTo) :
            from_(ntohl(inet_addr(ipFrom))),
            to_(ntohl(inet_addr(ipTo))) {}
    };

    static const int rangeCount = 4;
    static const AddressRange addressRange[rangeCount] = {
        AddressRange("10.0.0.0", "10.255.255.255"),
        AddressRange("172.16.0.0", "172.31.255.255"),
        AddressRange("169.254.0.0", "169.254.255.255"),
        AddressRange("192.168.0.0", "192.168.255.255")
    };

    const ACE_UINT32 ip = address.get_ip_address();
    for (int i = 0; i < rangeCount; ++i) {
        const AddressRange& range = addressRange[i];
        if ((range.from_ <= ip) && (ip <= range.to_)) {
            return true;
        }
    }
    return false;
}


bool isPublicAddress(const ACE_INET_Addr& address)
{
    return (! address.is_loopback()) && (! isPrivateAddress(address));
}


srpc::String obtainPublicIpAddress()
{
    const InetAddresses addresses = nsrpc::getLocalAddresses(0);

    InetAddresses::const_iterator posPublic =
        std::find_if(addresses.begin(), addresses.end(),
        nsrpc::isPublicAddress);
    if (posPublic != addresses.end()) {
        return (*posPublic).get_host_addr();
    }

    InetAddresses::const_iterator posPrivate =
        std::find_if(addresses.begin(), addresses.end(),
        nsrpc::isPrivateAddress);
    if (posPrivate != addresses.end()) {
        return (*posPrivate).get_host_addr();
    }

    return "127.0.0.1";
}


void workaroundWinsockConnectionResetProblem(ACE_HANDLE socket)
{
#if defined (_WIN32)
    DWORD dwBytesReturned = 0;
    BOOL  bNewBehavior = FALSE; // disable
    const DWORD status = ::WSAIoctl(reinterpret_cast<ACE_SOCKET>(socket),
        SIO_UDP_CONNRESET, &bNewBehavior, sizeof(bNewBehavior), NULL, 0,
        &dwBytesReturned, NULL, NULL);
    if (status == SOCKET_ERROR) {
        NSRPC_LOG_ERROR("Failed to disable SIO_UDP_CONNRESET(%m)");
    }
#else
    SRPC_UNUSED_ARG(socket);
#endif
}

} // namespace nsrpc
