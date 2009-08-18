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
 * 패킷 헤더를 기록하거나 읽어들인다.
 * - 패킷을 변경(예, 암호화)하려면 이 클래스를 상속받아 처리해야 한다.
 */
class NSRPC_API PacketCoder : public boost::noncopyable
{
public:
    typedef srpc::String Seed;
public:
    virtual ~PacketCoder() {}

    /// 초기화를 한다.
    virtual void reset() = 0;

    /// 암호화를 위한 시드값을 얻는다.
    virtual void generateCipherSeed(Seed& encryptSeed,
        Seed& decryptSeed) const = 0;

    /// base를 기반으로 암호화 키를 생성한다.
    virtual Seed generateCipherSeed(srpc::UInt32 base) const;

    /// 암호화를 위한 시드값을 설정한다
    virtual void setEncryptSeed(const Seed& seed) = 0;

    /// 복호화를 위한 시드값을 설정한다
    virtual void setDecryptSeed(const Seed& seed) = 0;

    /**
     * mblock에서 헤더 정보를 읽어온다.
     * @pre isHeadOfPacket()
     * @post isHeadOfPacket() && header.isValid()
     */
    virtual bool readHeader(AbstractPacketHeader& header,
        const ACE_Message_Block& mblock) const;

    /*
     * 패킷을 인코딩한다.
     * @param header header.bodySize_가 0이면 새로운 값으로 채워진다.
     * @pre isHeadOfPacket()
     * @post isHeadOfPacket() && header.isValid()
     */
    virtual bool encode(ACE_Message_Block& mblock,
        AbstractPacketHeader& header);

    /**
     * 패킷을 디코딩한다.
     * @pre isHeadOfPacket()
     * @post isHeadOfPacket()
     */
    virtual bool decode(ACE_Message_Block& mblock);

    /// 암호키 유효 기간을 연장한다
    virtual void extendCipherKeyTimeLimit() = 0;

    /// 패킷 헤더 크기
    virtual size_t getHeaderSize() const = 0;

    ///< 패킷의 기본 메모리 크기
    virtual size_t getDefaultPacketSize() const = 0;

    ///< 패킷의 최대 메모리 크기
    virtual size_t getMaxPacketSize() const = 0;

    ///< 패킷 풀의 크기
    virtual size_t getDefaultPacketPoolSize() const = 0;

    /// 암호화 초기 키값을 교환해야 하는가?
    virtual bool shouldExchangeCipherSeed() const = 0;

    /// 암호화 키의 유효기간이 만료되었는가?
    virtual bool isCipherKeyExpired() const = 0;

    /// 패킷 암호화 알고리즘을 가지고 있는가?
    virtual bool hasPacketCipher() const {
        return false;
    }
public:
    /// 패킷 헤더를 위한 공간을 예약한다.
    void reserveHeader(ACE_Message_Block& mblock);

    /// rd_ptr()를 packet body에 위치시킨다.
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
