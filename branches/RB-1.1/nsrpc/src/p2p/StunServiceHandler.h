#ifndef NSRPC_STUNSERVICEHANDLER_H
#define NSRPC_STUNSERVICEHANDLER_H

#include <srpc/Types.h>
#include <boost/noncopyable.hpp>

class ACE_Message_Block;

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class StunServiceHandler
 *
 * P2P STUN service handler
 */
class StunServiceHandler : public boost::noncopyable
{
public:
    virtual ~StunServiceHandler() {}

    /// address resolved
    virtual void resolved(const srpc::String& ipAddress, srpc::UInt16 port) = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_STUNSERVICEHANDLER_H
