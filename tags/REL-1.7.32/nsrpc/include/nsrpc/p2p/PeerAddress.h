#ifndef NSRPC_PEERADDRESS_H
#define NSRPC_PEERADDRESS_H

#include <srpc/Types.h>
#include <srpc/StringTypes.h>
#include <srpc/ContainerTypes.h>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct PeerAddress
 * Address info.
 */
struct PeerAddress
{
    srpc::String ip_;
    srpc::UInt16 port_;

    PeerAddress() :
        port_(0) {}

    PeerAddress(const srpc::String& ip, srpc::UInt16 port) :
        ip_(ip),
        port_(port) {}

    bool isValid() const {
        return (port_ > 0) && (! ip_.empty());
    }
};

/// IP addresses
typedef srpc::Vector<PeerAddress> PeerAddresses;

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERADDRESS_H
