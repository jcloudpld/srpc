#ifndef NSRPC_P2PPACKETCODER_H
#define NSRPC_P2PPACKETCODER_H

#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/p2p/detail/P2pProtocol.h>

namespace nsrpc
{

/** @addtogroup protocol
* @{
*/

/**
 * @class P2pPacketCoder
 * P2P Àü¿ë PacketCoder
 */
class P2pPacketCoder : public PacketCoder
{
    enum {
        defaultPacketSize = (P2pConfig::defaultMtu * 2),
        defaultPacketPoolSize = 10
    };
public:
    virtual ~P2pPacketCoder() {}

    virtual void reset() {}

    virtual void setEncryptSeed(const Seed& /*seed*/) {}

    virtual void setDecryptSeed(const Seed& /*seed*/) {}

    virtual void generateCipherSeed(Seed& encryptSeed,
        Seed& decryptSeed) const {
        encryptSeed.clear();
        decryptSeed.clear();
    }

    virtual void extendCipherKeyTimeLimit() {}

    virtual size_t getHeaderSize() const {
        return P2pPacketHeader::getHeaderSize();
    }

    virtual size_t getDefaultPacketSize() const {
        return defaultPacketSize;
    }

    virtual size_t getDefaultPacketPoolSize() const {
        return defaultPacketPoolSize;
    }

    virtual bool shouldExchangeCipherSeed() const {
        return false;
    }

    virtual bool isCipherKeyExpired() const {
        return false;
    }
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // !defined(NSRPC_P2PPACKETCODER_H)
