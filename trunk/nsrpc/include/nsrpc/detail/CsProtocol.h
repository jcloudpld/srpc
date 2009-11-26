#ifndef NSRPC_CSPROTOCOL_H
#define NSRPC_CSPROTOCOL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "Protocol.h"

namespace nsrpc
{

/** @addtogroup protocol
* @{
*/

enum CsMessageType
{
    mtUnknown = -1,
    mtSystem,
    mtGeneral
};

inline bool isValidCsMessageType(CsMessageType mt)
{
    return (mtSystem <= mt) && (mt <= mtGeneral);
}


/**
 * @struct CsPacketHeader
 * C/S 구조를 위한 패킷 헤더
 * - header := <body-size> + <message-type>
 */
struct NSRPC_API CsPacketHeader : public AbstractPacketHeader
{
    CsMessageType messageType_;

    explicit CsPacketHeader(CsMessageType messageType = mtGeneral) :
        messageType_(messageType) {}

    static size_t getHeaderSize() {
        return AbstractPacketHeader::getHeaderSize() + sizeof(srpc::UInt8);
    }

    virtual void reset() {
        AbstractPacketHeader::reset();
        messageType_ = mtUnknown;
    }

    virtual bool isValid() const {
        if (! AbstractPacketHeader::isValid()) {
            return false;
        }
        return isValidCsMessageType(messageType_);
    }

    virtual size_t write(void* buffer) const {
        assert(isValidCsMessageType(messageType_));
        char* to = static_cast<char*>(buffer);
        to += AbstractPacketHeader::write(to);
        to += packet::put(static_cast<srpc::UInt8>(messageType_), to);
        return to - static_cast<char*>(buffer);
    }

    virtual size_t read(const void* buffer) {
        const char* from = static_cast<const char*>(buffer);
        from += AbstractPacketHeader::read(from);
        srpc::UInt8 messageType;
        from += packet::get(messageType, from);
        messageType_ = static_cast<CsMessageType>(messageType);
        return from - static_cast<const char*>(buffer);
    }
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // !defined(NSRPC_CSPROTOCOL_H)
