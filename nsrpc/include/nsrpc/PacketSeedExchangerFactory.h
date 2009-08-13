#ifndef NSRPC_PACKETSEEDEXCHANGERFACTORY_H
#define NSRPC_PACKETSEEDEXCHANGERFACTORY_H

#include "nsrpc.h"

namespace nsrpc
{

class PacketSeedExchanger;

/**
 * @struct PacketSeedExchangerFactory
 */
struct NSRPC_API PacketSeedExchangerFactory
{
    static PacketSeedExchanger* createForServer();

    static PacketSeedExchanger* createForClient();
};

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETSEEDEXCHANGERFACTORY_H)
