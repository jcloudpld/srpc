#ifndef NSRPC_PACKETCODER_H
#define NSRPC_PACKETCODER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"
#include "Protocol.h"
#include <srpc/utility/Endian.h>
#include <srpc/StringTypes.h>
#include <ace/Message_Block.h>
#include <boost/noncopyable.hpp>

namespace nsrpc
{

/** @addtogroup protocol
* @{
*/

/**
 * @class PacketCoder
 * ��Ŷ ����� ����ϰų� �о���δ�.
 * - ��Ŷ�� ����(��, ��ȣȭ)�Ϸ��� �� Ŭ������ ��ӹ޾� ó���ؾ� �Ѵ�.
 */
class NSRPC_API PacketCoder : public boost::noncopyable
{
public:
    typedef srpc::String Seed;
public:
    virtual ~PacketCoder() {}

    /// �ʱ�ȭ�� �Ѵ�.
    virtual void reset() = 0;

    /// ��ȣȭ�� ���� �õ尪�� ��´�.
    virtual void generateCipherSeed(Seed& encryptSeed,
        Seed& decryptSeed) const = 0;

    /// base�� ������� ��ȣȭ Ű�� �����Ѵ�.
    virtual Seed generateCipherSeed(srpc::UInt32 base) const;

    /// ��ȣȭ�� ���� �õ尪�� �����Ѵ�
    virtual void setEncryptSeed(const Seed& seed) = 0;

    /// ��ȣȭ�� ���� �õ尪�� �����Ѵ�
    virtual void setDecryptSeed(const Seed& seed) = 0;

    /**
     * mblock���� ��� ������ �о�´�.
     * @pre isHeadOfPacket()
     * @post isHeadOfPacket() && header.isValid()
     */
    virtual bool readHeader(AbstractPacketHeader& header,
        const ACE_Message_Block& mblock) const;

    /*
     * ��Ŷ�� ���ڵ��Ѵ�.
     * @param header header.bodySize_�� 0�̸� ���ο� ������ ä������.
     * @pre isHeadOfPacket()
     * @post isHeadOfPacket() && header.isValid()
     */
    virtual bool encode(ACE_Message_Block& mblock,
        AbstractPacketHeader& header);

    /**
     * ��Ŷ�� ���ڵ��Ѵ�.
     * @pre isHeadOfPacket()
     * @post isHeadOfPacket()
     */
    virtual bool decode(ACE_Message_Block& mblock);

    /// ��ȣŰ ��ȿ �Ⱓ�� �����Ѵ�
    virtual void extendCipherKeyTimeLimit() = 0;

    /// ��Ŷ ��� ũ��
    virtual size_t getHeaderSize() const = 0;

    ///< ��Ŷ�� �⺻ �޸� ũ��
    virtual size_t getDefaultPacketSize() const = 0;

    ///< ��Ŷ�� �ִ� �޸� ũ��
    virtual size_t getMaxPacketSize() const = 0;

    ///< ��Ŷ Ǯ�� ũ��
    virtual size_t getDefaultPacketPoolSize() const = 0;

    /// ��ȣȭ �ʱ� Ű���� ��ȯ�ؾ� �ϴ°�?
    virtual bool shouldExchangeCipherSeed() const = 0;

    /// ��ȣȭ Ű�� ��ȿ�Ⱓ�� ����Ǿ��°�?
    virtual bool isCipherKeyExpired() const = 0;

    /// ��Ŷ ��ȣȭ �˰����� ������ �ִ°�?
    virtual bool hasPacketCipher() const {
        return false;
    }
public:
    /// ��Ŷ ����� ���� ������ �����Ѵ�.
    void reserveHeader(ACE_Message_Block& mblock);

    /// rd_ptr()�� packet body�� ��ġ��Ų��.
    void advanceToBody(ACE_Message_Block& mblock) {
        mblock.rd_ptr(mblock.base() + getHeaderSize());
    }

    bool isValidPacket(const ACE_Message_Block& mblock) const {
        return mblock.length() > getHeaderSize();
    }

    bool isValidHeader(const AbstractPacketHeader& header,
        ACE_Message_Block& mblock) const {
        return (mblock.length() - getHeaderSize()) ==
            static_cast<size_t>(header.bodySize_);
    }
protected:
    BodySize getBodySize(const ACE_Message_Block& mblock) const {
        return static_cast<BodySize>(mblock.length() - getHeaderSize());
    }
private:
    bool isValidPacketHeader(const ACE_Message_Block& mblock) const {
        return mblock.length() >= getHeaderSize();
    }

    bool isHeadOfPacket(const ACE_Message_Block& mblock) const {
        return mblock.rd_ptr() == mblock.base();
    }
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETCODER_H)
