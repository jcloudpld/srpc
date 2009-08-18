#ifndef NSRPC_PACKETSEEDEXCHANGERIMPL_H
#define NSRPC_PACKETSEEDEXCHANGERIMPL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/PacketSeedExchanger.h>

namespace nsrpc
{

/**
 * @class PacketSeedExchangerForServer
 */
class PacketSeedExchangerForServer : public PacketSeedExchanger
{
private:
    virtual void exchangePublicKey();
    virtual void exchangeNextSeed();

private:
    /// 초기 시드값을 교환한다
    void exchangeFirstSeed(const srpc::String& publicKey);

    void exchangeSeed(const srpc::String& publicKey);

public:
    // = RpcSecurityService
    OVERRIDE_SRPC_METHOD_1(exchangePublicKey,
        srpc::RShortString, publicKey);
    OVERRIDE_SRPC_METHOD_3(exchangeSeed,
        srpc::RShortString, publicKey,
        srpc::RShortString, seedForEncrypt,
        srpc::RShortString, seedForDecrypt);
    OVERRIDE_SRPC_METHOD_0(onConfirmSeed);

private:
    srpc::String privateKey_;
    srpc::String otherSidePublicKey_;
    srpc::String exchangingDecryptSeed_;
};


/**
* @class PacketSeedExchangerForClient
*/
class PacketSeedExchangerForClient : public PacketSeedExchanger
{
private:
    virtual void exchangePublicKey();

    virtual void exchangeNextSeed() {
        // NOP
    }

public:
    // = RpcSecurityService
    OVERRIDE_SRPC_METHOD_1(exchangePublicKey,
        srpc::RShortString, publicKey);
    OVERRIDE_SRPC_METHOD_3(exchangeSeed,
        srpc::RShortString, publicKey,
        srpc::RShortString, seedForEncrypt,
        srpc::RShortString, seedForDecrypt);
    OVERRIDE_SRPC_METHOD_0(onConfirmSeed);

private:
    srpc::String privateKey_;
    srpc::String otherSidePublicKey_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETSEEDEXCHANGERIMPL_H)
