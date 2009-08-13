#ifndef NSRPC_PEERHINT_H
#define NSRPC_PEERHINT_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/p2p/Group.h>
#include <nsrpc/p2p/P2pConfig.h>
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
    GroupId groupId_; ///< from/to GroupId
    const void* address_; ///< @internal
    bool isCandidate_; ///< @internal
    P2pOptions p2pOptions_; ///< @internal

    PeerHint(PeerId peerId,
        const void* address = 0,
        bool isCandidate = false) :
        peerId_(peerId),
        groupId_(giUnknown),
        address_(address),
        isCandidate_(isCandidate),
        p2pOptions_(poNone) {}

    PeerHint(GroupId groupId) :
        peerId_(invalidPeerId),
        groupId_(groupId),
        address_(0),
        isCandidate_(false),
        p2pOptions_(poNone) {}
};


inline const PeerHint& toPeerHint(const void* rpcHint)
{
    assert(rpcHint != 0);
    return *static_cast<const PeerHint*>(rpcHint);
}

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERHINT_H
