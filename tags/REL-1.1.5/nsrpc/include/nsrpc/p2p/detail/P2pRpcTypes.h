#ifndef NSRPC_P2PRPCTYPES_H
#define NSRPC_P2PRPCTYPES_H

#include "P2pAddress.h"
#include <srpc/RpcContainers.h>
#include <boost/scoped_array.hpp>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct RP2pProperty
 * P2P 속성
 */
struct RP2pProperty
{
    srpc::RUInt8 maxPeers_;
    srpc::RShortString sessionPassword_;
    srpc::RBool hostMigration_;
    srpc::RUInt32 sessionKey_;

    RP2pProperty() {
        reset();
    }

    void reset() {
        maxPeers_ = 0;
        sessionPassword_.clear();
        hostMigration_ = false;
        sessionKey_ = 0;
    }

    void write(srpc::OStream& ostream) {
        maxPeers_.write(ostream);
        sessionPassword_.write(ostream);
        hostMigration_.write(ostream);
        sessionKey_.write(ostream);
    }

    void read(srpc::IStream& istream) {
        maxPeers_.read(istream);
        sessionPassword_.read(istream);
        hostMigration_.read(istream);
        sessionKey_.read(istream);
    }
};

/**
 * @struct RAddress
 * Peer의 주소 정보
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

    RAddress& operator = (const ACE_INET_Addr& address) {
        set(address);
        return *this;
    }

    void write(srpc::OStream& ostream) {
        srpc::RUInt32(get_ip_address()).write(ostream);
        srpc::RUInt16(get_port_number()).write(ostream);
    }

    void read(srpc::IStream& istream) {
        srpc::RUInt32 ip;
        ip.read(istream);
        srpc::RUInt16 port;
        port.read(istream);
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
 * @todo TODO: 좀더 효율적으로 할 방법이 없을까?
 */
struct RMessageBuffer
{
    typedef boost::scoped_array<srpc::UInt8> BufferType;

    BufferType buffer_;
    srpc::UInt16 bufferSize_;
    srpc::UInt16 allocSize_;

    RMessageBuffer() :
        bufferSize_(0),
        allocSize_(0) {}

    RMessageBuffer(const RMessageBuffer& rhs) :
        allocSize_(0) {
        set(rhs.buffer_.get(), rhs.bufferSize_);
    }

    RMessageBuffer(const void* buffer, srpc::UInt16 length) :
        allocSize_(0) {
        set(buffer, length);
    }

    void set(const void* buffer, srpc::UInt16 length) {
        bufferSize_ = length;
        if (bufferSize_ > 0) {
            allocate(bufferSize_);
            memcpy(buffer_.get(), buffer, bufferSize_);
        }
    }

    void write(srpc::OStream& ostream) {
        ostream.write(bufferSize_);
        if (bufferSize_ > 0) {
            assert(buffer_.get() != 0);
            ostream.write(buffer_.get(), bufferSize_);
        }
    }

    void read(srpc::IStream& istream) {
        istream.read(bufferSize_);
        if (bufferSize_ > 0) {
            allocate(bufferSize_);
            istream.read(buffer_.get(), bufferSize_);
        }
    }
private:
    void allocate(srpc::UInt16 bufferSize) {
        if ((bufferSize == 0) || (allocSize_ >= bufferSize)) {
            assert(buffer_.get() != 0);
            return;
        }

        BufferType newBuffer(new srpc::UInt8[bufferSize]);
        buffer_.swap(newBuffer);
        allocSize_ = bufferSize;
    }
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PRPCTYPES_H
