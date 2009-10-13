#include "stdafx.h"
#include <nsrpc/detail/PacketCoderFactory.h>
#include "CsPacketCoder.h"
#include "p2p/P2pPacketCoder.h"

namespace nsrpc
{

PacketCoder* PacketCoderFactory::create() const
{
    return new CsPacketCoder;
}


PacketCoder* PacketCoderFactory::createForP2p() const
{
    return new P2pPacketCoder;
}

} // namespace nsrpc
