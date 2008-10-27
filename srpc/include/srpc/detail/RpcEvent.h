#ifndef SRPC_RPCEVENT_H
#define SRPC_RPCEVENT_H

#include "ReceivingFunctors.h"
#include "RpcId.h"
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
    RpcEvent(const RRpcId& rpcId) :
        rpcId_(rpcId) {}

    virtual ~RpcEvent() {}

    void dispatch(void* rpcClassThisPtr, IStream& istream,
        const void* rpcHint = 0) {
        ReceivingFunctor& dispatcher = getDispatcher();
        dispatcher.unmarshal(istream);
        dispatcher.call(rpcClassThisPtr, rpcHint);
    }

    const RRpcId& getRpcId() const {
        return rpcId_;
    }
public:
    virtual RpcEvent* clone() const = 0;

    virtual ReceivingFunctor& getDispatcher() = 0;
private:
    const RRpcId& rpcId_;
};

/** @} */ // addtogroup RpcReceiving

} // namespace srpc

#endif // SRPC_RPCEVENT_H
