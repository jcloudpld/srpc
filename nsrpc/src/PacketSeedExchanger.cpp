#include "stdafx.h"
#include <nsrpc/PacketSeedExchanger.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/SessionRpcNetwork.h>

namespace nsrpc
{

namespace
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
    DECLARE_SRPC_METHOD_2(exchangeSeed,
        srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);
    DECLARE_SRPC_METHOD_1(onConfirmSeed, srpc::RShortString, encryptSeed);
private:
    std::string exchangingDecryptSeed_;
};


void PacketSeedExchangerForServer::exchangeFirstSeed()
{
    exchangingDecryptSeed_.clear();
    if (getPacketCoder().shouldExchangeCipherSeed()) {
        exchangeSeed();
    }
}


void PacketSeedExchangerForServer::exchangeNextSeed()
{
    if (getPacketCoder().isCipherKeyExpired()) {
        exchangeSeed();
        getPacketCoder().extendCipherKeyTimeLimit();
    }
}


void PacketSeedExchangerForServer::exchangeSeed()
{
    if (! exchangingDecryptSeed_.empty()) {
        return;
    }

    PacketCoder::Seed encryptSeed, decryptSeed;
    getPacketCoder().generateCipherSeed(encryptSeed, decryptSeed);
    const CsMessageType messageType = mtSystem;
    exchangeSeed(encryptSeed, decryptSeed, &messageType);
    getPacketCoder().setEncryptSeed(encryptSeed);

    exchangingDecryptSeed_ = decryptSeed;
}


DEFINE_SRPC_METHOD_2(PacketSeedExchangerForServer, exchangeSeed,
    srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);


IMPLEMENT_SRPC_METHOD_1(PacketSeedExchangerForServer, onConfirmSeed,
    srpc::RShortString, encryptSeed)
{
    assert(getPacketCoder().shouldExchangeCipherSeed());

    getPacketCoder().setDecryptSeed(encryptSeed);

    if (exchangingDecryptSeed_ == encryptSeed) {
        exchangingDecryptSeed_.clear();
    }
}


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
    DECLARE_SRPC_METHOD_2(exchangeSeed,
        srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);
    DECLARE_SRPC_METHOD_1(onConfirmSeed, srpc::RShortString, encryptSeed);
};


IMPLEMENT_SRPC_METHOD_2(PacketSeedExchangerForClient, exchangeSeed,
    srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed)
{
    getPacketCoder().setDecryptSeed(encryptSeed);
    const CsMessageType messageType = mtSystem;
    onConfirmSeed(decryptSeed, &messageType);
    getPacketCoder().setEncryptSeed(decryptSeed);
}


DEFINE_SRPC_METHOD_1(PacketSeedExchangerForClient, onConfirmSeed,
    srpc::RShortString, encryptSeed);

} // namespace

// = PacketSeedExchanger

IMPLEMENT_SRPC_EVENT_DISPATCHER(PacketSeedExchanger);

void PacketSeedExchanger::initialize(PacketCoder* packetCoder,
        SessionRpcNetwork* rpcNetwork)
{
    assert(packetCoder != 0);
    assert(rpcNetwork != 0);

    packetCoder_ = packetCoder;

    srpc::RpcReceiver::setRpcNetwork(rpcNetwork);
    srpc::RpcForwarder::setRpcNetwork(rpcNetwork);
}


PacketSeedExchanger* PacketSeedExchanger::createForServer()
{
    return new PacketSeedExchangerForServer;
}


PacketSeedExchanger* PacketSeedExchanger::createForClient()
{
    return new PacketSeedExchangerForClient;
}

} // namespace nsrpc
