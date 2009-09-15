#ifndef NSRPC_PEERID_H
#define NSRPC_PEERID_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <srpc/RpcTypes.h>
#include <srpc/RpcContainers.h>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/// Peer id (must be unique in this session)
typedef srpc::UInt32 PeerId;
const PeerId invalidPeerId = 0;
const PeerId pseudoHostPeerId = 0xFFFFFFFE;
const PeerId relayServerPeerId = 0xFFFFFFFF;

inline bool isValidPeerId(PeerId peerId)
{
    return invalidPeerId != peerId;
}


inline bool isAllowedPeerId(PeerId peerId)
{
    return (invalidPeerId < peerId) && (peerId < pseudoHostPeerId);
}


inline bool isBroadcast(PeerId peerId)
{
    return ! isValidPeerId(peerId);
}


inline bool isRelayServer(PeerId peerId)
{
    return relayServerPeerId == peerId;
}


typedef srpc::Vector<PeerId> PeerIds;

typedef srpc::RpcUIntType<PeerId> RPeerId;
typedef srpc::RVector<RPeerId> RPeerIds;

/**
 * @struct PeerIdPair
 */
struct PeerIdPair
{
    PeerId from_;
    PeerId to_;

    PeerIdPair() :
        from_(invalidPeerId),
        to_(invalidPeerId) {}

    PeerIdPair(PeerId from, PeerId to) :
        from_(from),
        to_(to) {}
};


/**
 * @struct RPeerIdPair
 */
struct RPeerIdPair : PeerIdPair
{
    RPeerIdPair() {}

    RPeerIdPair(PeerId from, PeerId to) :
        PeerIdPair(from, to) {}

    RPeerIdPair(const PeerIdPair& rhs) :
        PeerIdPair(rhs) {}

    template <typename Stream>
    void serialize(Stream& ostream) {
        ostream & from_ & to_;
    }
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERID_H
