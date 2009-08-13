#ifndef NSRPC_P2PRPCTYPES_H
#define NSRPC_P2PRPCTYPES_H

#include "P2pAddress.h"
#include "../P2pConfig.h"
#include "../PeerId.h"
#include <srpc/RpcContainers.h>
#include <boost/scoped_array.hpp>

namespace nsrpc
{

namespace detail
{

typedef srpc::RpcUIntType<P2pOptions, 8> RP2pOptions;

/**
 * @struct RP2pProperty
 * P2P 加己
 */
struct RP2pProperty
{
    srpc::UInt8 maxPeers_;
    srpc::RShortString sessionPassword_;
    bool hostMigration_;
    srpc::UInt32 sessionKey_;
    RPeerIds hostPrecedence_;

    RP2pProperty() {
        reset();
    }

    void reset() {
        maxPeers_ = 0;
        sessionPassword_.clear();
        hostMigration_ = false;
        sessionKey_ = 0;
        hostPrecedence_.clear();
    }

    template <typename Stream>
    void serialize(Stream& ostream) {
        ostream & maxPeers_ & sessionPassword_ & hostMigration_ &
            sessionKey_ & hostPrecedence_;
    }
};

/**
 * @struct RAddress
 * Peer狼 林家 沥焊
 */
struct RAddress : ACE_INET_Addr
{
    RAddress() {}

    RAddress(srpc::UInt32 ip, srpc::UInt16 port) {
        set(port, ip);
    }

    RAddress(const ACE_INET_Addr& address) {
        set(address);
    }

    RAddress& operator=(const ACE_INET_Addr& address) {
        set(address);
        return *this;
    }

    void serialize(srpc::OStream& ostream) {
        ostream.write(srpc::UInt32(get_ip_address()));
        ostream.write(srpc::UInt16(get_port_number()));
    }

    void serialize(srpc::IStream& istream) {
        srpc::UInt32 ip;
        istream.read(ip);
        srpc::UInt16 port;
        istream.read(port);
        set(port, ip);
    }
};


/// RpcType for PeerAddress list
typedef srpc::RVector<RAddress> RAddresses;


/// convert to RAddresses
inline RAddresses toAddresses(const Addresses& addresses)
{
    RAddresses peerAddresses;
    peerAddresses.reserve(addresses.size());

    Addresses::const_iterator pos = addresses.begin();
    const Addresses::const_iterator end = addresses.end();
    for (; pos != end; ++pos) {
        peerAddresses.push_back(RAddress(*pos));
    }
    return peerAddresses;
}


/// convert to Addresses
inline Addresses toAddresses(const RAddresses& peerAddresses)
{
    Addresses addresses;
    addresses.assign(peerAddresses.begin(), peerAddresses.end());
    return addresses;
}


inline PeerAddress toPeerAddress(const RAddress& address)
{
    return PeerAddress(address.get_host_addr(),
        address.get_port_number());
}


inline PeerAddresses toPeerAddresses(const RAddresses& addresses)
{
    PeerAddresses peerAddresses;
    peerAddresses.reserve(addresses.size());

    RAddresses::const_iterator pos = addresses.begin();
    const RAddresses::const_iterator end = addresses.end();
    for (; pos != end; ++pos) {
        peerAddresses.push_back(toPeerAddress(*pos));
    }
    return peerAddresses;
}


/**
 * @struct RMessageBuffer
 * RpcType for raw memory buffer
 */
struct RMessageBuffer
{
    typedef srpc::UInt16 LengthType;
    typedef srpc::UInt8 DataType;

    const static LengthType maxBufferLength = P2pConfig::defaultMtu * 2;

    explicit RMessageBuffer(const void* buffer = 0, size_t length = 0) {
        assign(buffer, static_cast<LengthType>(length));
    }

    RMessageBuffer(const RMessageBuffer& rhs) {
        assign(rhs);
    }

    RMessageBuffer& operator=(const RMessageBuffer& rhs) {
        assign(rhs);
        return *this;
    }

    const void* getBuffer() const {
        return buffer_;
    }

    LengthType getBufferLength() const {
        return bufferLength_;
    }

    void serialize(srpc::OStream& ostream) {
        assert(bufferLength_ <= maxBufferLength);
        ostream.write(bufferLength_);
        ostream.write(buffer_, bufferLength_);
    }

    void serialize(srpc::IStream& istream) {
        istream.read(bufferLength_);
        assert(bufferLength_ <= maxBufferLength);
        istream.read(buffer_, bufferLength_);
    }
private:
    void copyBuffer(const void* buffer, srpc::UInt16 bufferLen) {
        memcpy(buffer_, buffer, bufferLen);
    }

    void assign(const RMessageBuffer& message) {
        assign(message.buffer_, message.bufferLength_);
    }

    void assign(const void* buffer, LengthType bufferLen) {
        assert(bufferLen <= maxBufferLength);
        bufferLength_ = bufferLen;
        copyBuffer(buffer, bufferLen);
    }
private:
    DataType buffer_[maxBufferLength];
    LengthType bufferLength_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PRPCTYPES_H
