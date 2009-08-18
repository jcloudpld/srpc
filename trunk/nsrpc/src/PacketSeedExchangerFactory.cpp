#include "stdafx.h"
#include "PacketSeedExchangerImpl.h"
#include <nsrpc/PacketSeedExchangerFactory.h>

namespace nsrpc
{

PacketSeedExchanger* PacketSeedExchangerFactory::createForServer()
{
    return new PacketSeedExchangerForServer();
}


PacketSeedExchanger* PacketSeedExchangerFactory::createForClient()
{
    return new PacketSeedExchangerForClient();
}

} // namespace nsrpc
