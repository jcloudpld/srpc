#ifndef NSRPC_P2PPEERHINT_H
#define NSRPC_P2PPEERHINT_H

#include "../PeerHint.h"
#include <ace/INET_Addr.h>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct P2pPeerHint
 */
struct P2pPeerHint : PeerHint
{
    P2pPeerHint(PeerId peerId,
        const void* address = 0,
        bool candidate = false) :
        PeerHint(peerId, address, candidate) {}

    bool isValid() const {
        return (invalidPeerId != peerId_) &&
            (address_ != 0) &&
            (static_cast<const ACE_INET_Addr*>(address_)->
                get_port_number() != 0);
    }

    ACE_INET_Addr getAddress() const {
        if (address_ != 0) {
            return *static_cast<const ACE_INET_Addr*>(address_);
        }
        return ACE_INET_Addr();
    }
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PPEERHINT_H
