#include "stdafx.h"
#include "PacketSeedExchangerImpl.h"
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/SessionRpcHint.h>

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

    PacketCoder::Seed encryptSeed;
    PacketCoder::Seed decryptSeed;
    getPacketCoder().generateCipherSeed(encryptSeed, decryptSeed);

    SessionRpcHint rpcHint(0, mtSystem);
    exchangeSeed(encryptSeed, decryptSeed, &rpcHint);
    getPacketCoder().setEncryptSeed(encryptSeed);

    exchangingDecryptSeed_ = decryptSeed;
}


FORWARD_SRPC_METHOD_2(PacketSeedExchangerForServer, exchangeSeed,
    srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);


RECEIVE_SRPC_METHOD_1(PacketSeedExchangerForServer, onConfirmSeed,
    srpc::RShortString, encryptSeed)
{
    assert(getPacketCoder().shouldExchangeCipherSeed());

    getPacketCoder().setDecryptSeed(encryptSeed);

    if (exchangingDecryptSeed_ == encryptSeed) {
        exchangingDecryptSeed_.clear();
    }
}

// = PacketSeedExchangerForClient

RECEIVE_SRPC_METHOD_2(PacketSeedExchangerForClient, exchangeSeed,
    srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed)
{
    getPacketCoder().setDecryptSeed(encryptSeed);
    SessionRpcHint rpcHint(0, mtSystem);
    onConfirmSeed(decryptSeed, &rpcHint);
    getPacketCoder().setEncryptSeed(decryptSeed);
}


FORWARD_SRPC_METHOD_1(PacketSeedExchangerForClient, onConfirmSeed,
    srpc::RShortString, encryptSeed);

} // namespace nsrpc
