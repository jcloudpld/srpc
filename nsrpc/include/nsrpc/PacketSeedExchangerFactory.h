#ifndef NSRPC_PACKETSEEDEXCHANGERFACTORY_H
#define NSRPC_PACKETSEEDEXCHANGERFACTORY_H

#ifdef _MSC_VER
#  pragma once
#endif

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
