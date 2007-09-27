#ifndef NSRPC_PEERADDRESS_H
#define NSRPC_PEERADDRESS_H

#include <srpc/Types.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <vector>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

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
    std::string ip_;
    srpc::UInt16 port_;

    PeerAddress() :
        port_(0) {}

    PeerAddress(const std::string& ip, srpc::UInt16 port) :
        ip_(ip),
        port_(port) {}

    bool isValid() const {
        return (port_ > 0) && (! ip_.empty());
    }
};

/// IP addresses
typedef std::vector<PeerAddress> PeerAddresses;

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERADDRESS_H
