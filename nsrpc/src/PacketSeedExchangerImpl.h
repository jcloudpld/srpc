#ifndef NSRPC_PACKETSEEDEXCHANGERIMPL_H
#define NSRPC_PACKETSEEDEXCHANGERIMPL_H

#include <nsrpc/PacketSeedExchanger.h>

namespace nsrpc
{

/**
 * @class PacketSeedExchangerForServer
 */
class PacketSeedExchangerForServer : public PacketSeedExchanger
{
public:
    virtual void exchangeFirstSeed();
    virtual void exchangeNextSeed();
private:
    void exchangeSeed();
public:
    // = RpcSecurityService
    OVERRIDE_SRPC_METHOD_2(exchangeSeed,
        srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);
    OVERRIDE_SRPC_METHOD_1(onConfirmSeed, srpc::RShortString, encryptSeed);
private:
    srpc::String exchangingDecryptSeed_;
};


/**
* @class PacketSeedExchangerForClient
*/
class PacketSeedExchangerForClient : public PacketSeedExchanger
{
public:
    virtual void exchangeFirstSeed() {
        // NOP
    }

    virtual void exchangeNextSeed() {
        // NOP
    }
public:
    // = RpcSecurityService
    OVERRIDE_SRPC_METHOD_2(exchangeSeed,
        srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);
    OVERRIDE_SRPC_METHOD_1(onConfirmSeed, srpc::RShortString, encryptSeed);
};

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETSEEDEXCHANGERIMPL_H)
