#include "stdafx.h"
#include <nsrpc/p2p/detail/P2pEndpoint.h>
#include <nsrpc/p2p/detail/PeerNotifier.h>
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/utility/Logger.h>
#include <nsrpc/utility/AceUtil.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Reactor.h>
#include <cassert>

namespace nsrpc
{

namespace detail
{

// = P2pEndpoint

P2pEndpoint::P2pEndpoint(PeerNotifier& notifier,
    ACE_Message_Block& recvBlock, ACE_Reactor* reactor) :
    ACE_Event_Handler(reactor),
    notifier_(notifier),
    recvBlock_(recvBlock),
    udp_(new ACE_SOCK_Dgram)
{
}


P2pEndpoint::~P2pEndpoint()
{
    close();
}


bool P2pEndpoint::open(srpc::UInt16 port)
{
    const ACE_INET_Addr localAddr(port);
    const int reuse_addr = 1;

    if (udp_->open(localAddr, ACE_PROTOCOL_FAMILY_INET, 0, reuse_addr) == -1) {
        NSRPC_LOG_ERROR2(ACE_TEXT("Can't open a UDP socket(port:%d)"), port);
        return false;
    }
    setMaximumSocketBufferSize(get_handle());
    udp_->enable(ACE_NONBLOCK);

    const ACE_Reactor_Mask masks = ACE_Event_Handler::READ_MASK;
    if (reactor()->register_handler(this, masks) == -1) {
        NSRPC_LOG_ERROR2(
            ACE_TEXT("ACE_Reactor::register_handler() FAILED!!!(%d,%m)"), 
            ACE_OS::last_error());
        return false;
    }

    if (! setAddresses(port)) {
        return false;
    }

    return true;
}


void P2pEndpoint::close()
{
    ACE_Reactor_Mask masks = ACE_Event_Handler::ALL_EVENTS_MASK |
        ACE_Event_Handler::DONT_CALL;
    reactor()->remove_handler(this, masks);

    udp_->close();
    clearAddresses();

    NSRPC_LOG_DEBUG(ACE_TEXT("UDP socket closed"));
}


bool P2pEndpoint::send(const ACE_INET_Addr& peerAddr,
    const ACE_Message_Block& mblock)
{
    const size_t blockSize = mblock.length();
    const ssize_t sentSize =
        udp_->send(mblock.rd_ptr(), blockSize, peerAddr);
    if (sentSize != static_cast<ssize_t>(blockSize)) {
        if (ACE_OS::last_error() == EWOULDBLOCK) {
            return true;
        }

        NSRPC_LOG_ERROR4(
            ACE_TEXT("P2pEndpoint::send(%s:%d) FAILED!!!(%d,%m)"),
            peerAddr.get_host_addr(), peerAddr.get_port_number(),
            ACE_OS::last_error());
        return false;
    }
    return true;
}


void P2pEndpoint::read()
{
    if (! isOpened()) {
        return;
    }

    (void)handle_input(0);
}


bool P2pEndpoint::setAddresses(srpc::UInt16 port)
{
    clearAddresses();

    const InetAddresses addresses = nsrpc::getLocalAddresses(port);
    for (InetAddresses::const_iterator pos = addresses.begin();
        pos != addresses.end(); ++pos) {
        localAddresses_.push_back(*pos);
    }

    return true;
}


bool P2pEndpoint::readMessage(ACE_INET_Addr& peerAddr,
    ACE_Message_Block& mblock)
{
    assert(mblock.size() >= P2pConfig::defaultMtu);

    const ssize_t recvSize =
        udp_->recv(mblock.wr_ptr(), mblock.space(), peerAddr);
    if (recvSize == 0) {
        return false;
    }

    if (recvSize < 0) {
        const int error = ACE_OS::last_error();
        if (error == EWOULDBLOCK) {
            return false;
        }
        if (error == ECONNRESET) {
            return false;
        }

        NSRPC_LOG_ERROR4(
            ACE_TEXT("P2pEndpoint::readMessage(from: %s:%d) FAILED!!!(%d,%m)"),
            peerAddr.get_host_addr(), peerAddr.get_port_number(),
            ACE_OS::last_error());
        return false;
    }
    mblock.wr_ptr(recvSize);

    return true;
}


int P2pEndpoint::handle_input(ACE_HANDLE)
{
    recvBlock_.reset();

    ACE_INET_Addr peerAddr;
    if (readMessage(peerAddr, recvBlock_)) {
        notifier_.onMessageArrived(peerAddr);
    }

    //reactor()->notify(this, ACE_Event_Handler::READ_MASK);
    return 0;
}


int P2pEndpoint::handle_close(ACE_HANDLE, ACE_Reactor_Mask)
{
    close();
    return 0;
}


ACE_HANDLE P2pEndpoint::get_handle() const
{
    return udp_->get_handle();
}

} // namespace detail

} // namespace nsrpc
