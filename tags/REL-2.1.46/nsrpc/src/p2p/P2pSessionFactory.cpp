#include "stdafx.h"
#include <nsrpc/p2p/P2pSessionFactory.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include "P2pSessionImpl.h"

namespace nsrpc
{

P2pSession* P2pSessionFactory::create(PeerId peerId,
    P2pEventHandler& eventHandler, const P2pConfig& p2pConfig,
    bool useBitPacking, const PacketCoderFactory* packetCoderFactory)
{
    PacketCoder* packetCoder;
    if (packetCoderFactory != 0) {
        packetCoder = packetCoderFactory->createForP2p();
    }
    else {
        packetCoder = PacketCoderFactory().createForP2p();
    }

    return new detail::P2pSessionImpl(peerId, eventHandler, p2pConfig,
        useBitPacking, packetCoder);
}

} // namespace nsrpc
