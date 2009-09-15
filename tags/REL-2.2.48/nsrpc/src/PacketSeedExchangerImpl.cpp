#include "stdafx.h"
#include "PacketSeedExchangerImpl.h"
#include <nsrpc/PacketSeedExchangerCallback.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/SessionRpcHint.h>

namespace nsrpc
{

// = PacketSeedExchangerForServer

void PacketSeedExchangerForServer::exchangePublicKey()
{
    if (! getPacketCoder().hasPacketCipher()) {
        getCallback().seedExchanged();
    }
}


void PacketSeedExchangerForServer::exchangeNextSeed()
{
    if ((! getPacketCoder().shouldExchangeCipherSeed()) ||
        (! getPacketCoder().isCipherKeyExpired())) {
        return;
    }

    exchangeSeed(srpc::null_string);
    getPacketCoder().extendCipherKeyTimeLimit();
}


void PacketSeedExchangerForServer::exchangeFirstSeed(const srpc::String& publicKey)
{
    if (! getPacketCoder().shouldExchangeCipherSeed()) {
        return;
    }

    exchangingDecryptSeed_.clear();
    exchangeSeed(publicKey);
}


void PacketSeedExchangerForServer::exchangeSeed(const srpc::String& publicKey)
{
    if (! exchangingDecryptSeed_.empty()) {
        return;
    }

    PacketCoder::Seed seedForEncrypt;
    PacketCoder::Seed seedForDecrypt;
    getPacketCoder().generateCipherSeed(seedForEncrypt, seedForDecrypt);

    exchangingDecryptSeed_ = seedForDecrypt;

    const PacketCoder::Seed seedForEncryptTemp = seedForEncrypt;
    getCallback().encryptSeed(seedForEncrypt, otherSidePublicKey_);
    getCallback().encryptSeed(seedForDecrypt, otherSidePublicKey_);

    SessionRpcHint rpcHint(0, mtSystem);
    exchangeSeed(publicKey, seedForEncrypt, seedForDecrypt, &rpcHint);

    getPacketCoder().setEncryptSeed(seedForEncryptTemp);
}


RECEIVE_SRPC_METHOD_1(PacketSeedExchangerForServer, exchangePublicKey,
    srpc::RShortString, publicKey)
{
    if (! getPacketCoder().shouldExchangeCipherSeed()) {
        // 패킷 암호화는 사용하고, 키 교환은 하지 않는 경우
        SessionRpcHint rpcHint(0, mtSystem);
        exchangeSeed(srpc::null_string, srpc::null_string, srpc::null_string,
            &rpcHint);
        getCallback().seedExchanged();
        return;
    }

    otherSidePublicKey_ = publicKey;

    srpc::String myPublicKey;
    if (! getCallback().generatePublicAndPrivateKey(myPublicKey, privateKey_)) {
        assert(false && "why failed?");
        return;
    }

    exchangeFirstSeed(myPublicKey);
}


FORWARD_SRPC_METHOD_3(PacketSeedExchangerForServer, exchangeSeed,
    srpc::RShortString, publicKey,
    srpc::RShortString, seedForEncrypt, srpc::RShortString, seedForDecrypt);


RECEIVE_SRPC_METHOD_0(PacketSeedExchangerForServer, onConfirmSeed)
{
    assert(getPacketCoder().shouldExchangeCipherSeed());

    getPacketCoder().setDecryptSeed(exchangingDecryptSeed_);
    exchangingDecryptSeed_.clear();

    getCallback().seedExchanged();
}

// = PacketSeedExchangerForClient

void PacketSeedExchangerForClient::exchangePublicKey()
{
    if (! getPacketCoder().hasPacketCipher()) {
        getCallback().seedExchanged();
        return;
    }

    srpc::String publicKey;
    if (! getCallback().generatePublicAndPrivateKey(publicKey, privateKey_)) {
        getCallback().seedExchanged();
        return;
    }

    SessionRpcHint rpcHint(0, mtSystem);
    exchangePublicKey(publicKey, &rpcHint);
}


FORWARD_SRPC_METHOD_1(PacketSeedExchangerForClient, exchangePublicKey,
    srpc::RShortString, publicKey);


RECEIVE_SRPC_METHOD_3(PacketSeedExchangerForClient, exchangeSeed,
    srpc::RShortString, publicKey,
    srpc::RShortString, seedForEncrypt, srpc::RShortString, seedForDecrypt)
{
    if (publicKey.empty() && seedForEncrypt.empty() && seedForDecrypt.empty()) {
        // 패킷 암호화는 사용하고, 키 교환은 하지 않는 경우
        getCallback().seedExchanged();
        return;
    }

    otherSidePublicKey_ = publicKey;

    srpc::String decryptedSeedForEncrypt = seedForEncrypt;
    if (! getCallback().decryptSeed(decryptedSeedForEncrypt)) {
        assert(false && "why failed?");
        return;
    }

    srpc::String decryptedSeedForDecrypt = seedForDecrypt;
    if (! getCallback().decryptSeed(decryptedSeedForDecrypt)) {
        assert(false && "why failed?");
        return;
    }

    getPacketCoder().setDecryptSeed(decryptedSeedForEncrypt);

    SessionRpcHint rpcHint(0, mtSystem);
    onConfirmSeed(&rpcHint);

    getPacketCoder().setEncryptSeed(decryptedSeedForDecrypt);

    getCallback().seedExchanged();
}


FORWARD_SRPC_METHOD_0(PacketSeedExchangerForClient, onConfirmSeed);

} // namespace nsrpc
