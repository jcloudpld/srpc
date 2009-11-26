#ifndef NSRPC_CSPACKETCODER_H
#define NSRPC_CSPACKETCODER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/CsProtocol.h>

namespace nsrpc
{

/** @addtogroup protocol
* @{
*/

/**
 * @class CsPacketCoder
 * C/S Àü¿ë PacketCoder
 */
class CsPacketCoder : public PacketCoder
{
    enum {
        defaultPacketSize = 2 * 1024,
        defaultPacketPoolSize = 10
    };
public:
    virtual ~CsPacketCoder() {}

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
        return CsPacketHeader::getHeaderSize();
    }

    virtual size_t getDefaultPacketSize() const {
        return defaultPacketSize;
    }

    virtual size_t getMaxPacketSize() const {
        return std::numeric_limits<BodySize>::max();
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

#endif // !defined(NSRPC_CSPACKETCODER_H)
