#ifndef NSRPC_PEERHINT_H
#define NSRPC_PEERHINT_H

#include <nsrpc/p2p/PeerId.h>
#include <cassert>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct PeerHint
 * RPC hint for P2P
 */
struct PeerHint
{
    PeerId peerId_; ///< from/to PeerId
    const void* address_; ///< @internal
    bool candidate_; ///< @internal

    PeerHint(PeerId peerId,
        const void* address = 0,
        bool candidate = false) :
        peerId_(peerId),
        address_(address),
        candidate_(candidate) {}
};


inline const PeerHint& toPeerHint(const void* rpcHint)
{
    assert(rpcHint != 0);
    return *static_cast<const PeerHint*>(rpcHint);
}

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERHINT_H
