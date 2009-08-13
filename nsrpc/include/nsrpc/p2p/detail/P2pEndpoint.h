#ifndef NSRPC_P2PENDPOINT_H
#define NSRPC_P2PENDPOINT_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "P2pAddress.h"
#include "../PeerId.h"
#include "../../nsrpc.h"
#include <srpc/Types.h>
#include <ace/Event_Handler.h>
#include <boost/scoped_ptr.hpp>

class ACE_SOCK_Dgram;

namespace nsrpc
{

namespace detail
{

class PeerNotifier;

/** @addtogroup p2p
* @{
*/

/**
 * @class P2pEndpoint
 * P2P endpoint
 * - Reactor Event Handler
 */
class NSRPC_API P2pEndpoint : public ACE_Event_Handler
{
public:
    P2pEndpoint(PeerNotifier& notifier, ACE_Message_Block& recvBlock,
        ACE_Reactor* reactor);
    virtual ~P2pEndpoint();

    /**
     * 초기화한다.
     * @param port listening port
     */
    bool open(srpc::UInt16 port);

    /// 소켓을 닫는다.
    void close();

    /// 메세지를 전송한다
    bool send(const ACE_INET_Addr& peerAddr, const ACE_Message_Block& mblock);

    void read();

    bool isOpened() const {
        return get_handle() != ACE_INVALID_HANDLE;
    }

    /// local address를 얻는다.
    const Addresses& getLocalAddresses() const {
        return localAddresses_;
    }
private:
    bool setAddresses(srpc::UInt16 port);
    void clearAddresses() {
        localAddresses_.clear();
    }
    bool readMessage(ACE_INET_Addr& peerAddr, ACE_Message_Block& mblock);
private:
    // = ACE_Event_Handler overriding
    virtual int handle_input(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    //virtual int handle_output(ACE_HANDLE fd = ACE_INVALID_HANDLE);
    //virtual int handle_timeout(const ACE_Time_Value& time, const void *);
    virtual int handle_close(ACE_HANDLE, ACE_Reactor_Mask);
    virtual ACE_HANDLE get_handle() const;
private:
    PeerNotifier& notifier_;
    ACE_Message_Block& recvBlock_;
    boost::scoped_ptr<ACE_SOCK_Dgram> udp_;
    Addresses localAddresses_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PENDPOINT_H
