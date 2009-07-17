#ifndef NSRPC_P2PSESSIONFACTORY_H
#define NSRPC_P2PSESSIONFACTORY_H

#include "../nsrpc.h"
#include "P2pSession.h"
#include "P2pConfig.h"

namespace nsrpc
{

class P2pEventHandler;
class PacketCoderFactory;

/** @addtogroup p2p
* @{
*/

/**
 * @struct P2pSessionFactory
 * P2P Session Factory
 */
struct NSRPC_API P2pSessionFactory
{
    /**
     * P2pSession factory method
     * @param peerId my PeerId (isAllowedPeerId(peerId) must be true).
     * @param eventHandler P2P event handler.
     * @param p2pConfig P2P configuration.
     * @param useBitPacking use compressed packet?
     * @param packetCoderFactory factory for PacketCoder
     */
    static P2pSession* create(PeerId peerId,
        P2pEventHandler& eventHandler,
        const P2pConfig& p2pConfig = P2pConfig(),
        bool useBitPacking = true,
        const PacketCoderFactory* packetCoderFactory = 0);
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_P2PSESSIONFACTORY_H
