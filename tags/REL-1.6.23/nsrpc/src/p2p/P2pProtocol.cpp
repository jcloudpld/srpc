#include "stdafx.h"
#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <srpc/utility/Endian.h>

namespace nsrpc
{

size_t P2pPacketHeader::write(void* buffer) const
{
    char* to = static_cast<char*>(buffer);
    to += AbstractPacketHeader::write(to);
    to += packet::put(static_cast<srpc::UInt8>(packetType_), to);
    to += packet::put(peerId_, to);
    to += packet::put(sequenceNumber_, to);
    to += packet::put(sentTime_, to);
    return to - static_cast<char*>(buffer);
}


size_t P2pPacketHeader::read(const void* buffer)
{
    const char* from = static_cast<const char*>(buffer);
    from += AbstractPacketHeader::read(from);
    srpc::UInt8 packetType;
    from += packet::get(packetType, from);
    packetType_ = static_cast<srpc::RpcPacketType>(packetType);
    from += packet::get(peerId_, from);
    from += packet::get(sequenceNumber_, from);
    from += packet::get(sentTime_, from);
    return from - static_cast<const char*>(buffer);
}

} // namespace nsrpc
