#include "stdafx.h"
#include "PacketSeedExchangerImpl.h"
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/CsProtocol.h>

namespace nsrpc
{

// = PacketSeedExchangerForServer

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

// = PacketSeedExchangerForClient

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

} // namespace nsrpc
