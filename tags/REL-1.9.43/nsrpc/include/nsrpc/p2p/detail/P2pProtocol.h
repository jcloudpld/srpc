#ifndef NSRPC_P2PPROTOCOL_H
#define NSRPC_P2PPROTOCOL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "PeerTime.h"
#include "../../detail/Protocol.h"
#include "../PeerId.h"
#include <srpc/RpcPacketType.h>
#include <boost/noncopyable.hpp>

namespace nsrpc
{

/** @addtogroup protocol
* @{
*/

/// packet seqence number
typedef srpc::UInt32 SequenceNumber;

const SequenceNumber invalidSequenceNumber = 0;
const SequenceNumber firstSequenceNumber = invalidSequenceNumber + 1;


/**
* @class P2pPacketHeader
* P2P 패킷 헤더
* - message := <header> + <body>
* - header := <body-size> + <packet-type> + <peer-id> + <seq-number> +
*             <sent-time>
* - body-size := sizeof(body)
*/
struct NSRPC_API P2pPacketHeader : AbstractPacketHeader
{
    srpc::RpcPacketType packetType_;
    PeerId peerId_; ///< from PeerId
    SequenceNumber sequenceNumber_;
    PeerTime sentTime_;

    P2pPacketHeader() :
        packetType_(srpc::ptReliable),
        peerId_(invalidPeerId),
        sequenceNumber_(invalidSequenceNumber),
        sentTime_(0) {}

    P2pPacketHeader(srpc::RpcPacketType packetType,
        PeerId peerId, SequenceNumber sequenceNumber, PeerTime sentTime) :
        packetType_(packetType),
        peerId_(peerId),
        sequenceNumber_(sequenceNumber),
        sentTime_(sentTime) {}

    static size_t getHeaderSize() {
        return AbstractPacketHeader::getHeaderSize() +
            sizeof(srpc::UInt8) + sizeof(PeerId) + sizeof(SequenceNumber) +
            sizeof(PeerTime);
    }

    virtual size_t write(void* buffer) const;

    virtual size_t read(const void* buffer);
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // !defined(NSRPC_P2PPROTOCOL_H)
