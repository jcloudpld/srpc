#ifndef SRPC_RPCEVENT_H
#define SRPC_RPCEVENT_H

#include "ReceivingFunctors.h"
#include <boost/noncopyable.hpp>

namespace srpc {

class IStream;

/** @addtogroup RpcReceiving
* @{
*/

/**
 * @class RpcEvent
 *
 * rpc event(response)
 */
class RpcEvent : public boost::noncopyable
{
public:
    virtual ~RpcEvent() {}

    void dispatch(void* rpcClassThisPtr, IStream& istream,
        const void* rpcHint = 0) {
        ReceivingFunctor& dispatcher = getDispatcher();
        dispatcher.unmarshal(istream);
        dispatcher.call(rpcClassThisPtr, rpcHint);
    }
public:
    virtual RpcEvent* clone() const = 0;

    virtual ReceivingFunctor& getDispatcher() = 0;
};

/** @} */ // addtogroup RpcReceiving

} // namespace srpc

#endif // SRPC_RPCEVENT_H
