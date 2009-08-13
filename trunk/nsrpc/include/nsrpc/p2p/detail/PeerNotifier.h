#ifndef NSRPC_PEERNOTIFIER_H
#define NSRPC_PEERNOTIFIER_H

#ifdef _MSC_VER
#  pragma once
#endif

class ACE_INET_Addr;

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class PeerNotifier
 */
class PeerNotifier
{
public:
    virtual ~PeerNotifier() {}

    /// 메세지가 도착하였다
    virtual void onMessageArrived(const ACE_INET_Addr& peerAddr) = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERNOTIFIER_H
