#ifndef NSRPC_P2PADDRESS_H
#define NSRPC_P2PADDRESS_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../PeerAddress.h"
#include <srpc/ContainerTypes.h>
#ifdef _MSC_VER
#  pragma warning( push )
#  pragma warning( disable : 4127 4251 4541 4511 4512 )
#endif
#include <ace/INET_Addr.h>
#ifdef _MSC_VER
#  pragma warning( pop )
#endif

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/// IP addresses
typedef srpc::List<ACE_INET_Addr> Addresses;


/// convert from PeerAddresses to Addresses
inline Addresses toAddresses(const PeerAddresses& addrs)
{
    Addresses converted;
    PeerAddresses::const_iterator pos = addrs.begin();
    const PeerAddresses::const_iterator end = addrs.end();
    for (; pos != end; ++pos) {
        const PeerAddress& addr = *pos;
        converted.push_back(ACE_INET_Addr(addr.port_, addr.ip_.c_str()));
    }
    return converted;
}


/**
 * @struct AddressPair
 */
struct AddressPair
{
    ACE_INET_Addr targetAddress_;
    ACE_INET_Addr peerAddress_;

    AddressPair() {}

    AddressPair(const ACE_INET_Addr& targetAddress,
        const ACE_INET_Addr& peerAddress) :
        targetAddress_(targetAddress),
        peerAddress_(peerAddress) {}

    bool isSame() const {
        return targetAddress_ == peerAddress_;
    }
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PADDRESS_H
