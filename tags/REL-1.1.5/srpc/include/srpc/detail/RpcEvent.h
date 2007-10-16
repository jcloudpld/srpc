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
    RpcEvent(ReceivingFunctor& dispatcher) :
        dispatcher_(dispatcher) {}
    virtual ~RpcEvent() {}

    void dispatch(void* rpcClassThisPtr, IStream& istream,
        const void* rpcHint = 0) {
        dispatcher_(istream);
        dispatcher_.call(rpcClassThisPtr, rpcHint);
    }
public:
    virtual RpcEvent* clone() const = 0;
private:
    ReceivingFunctor& dispatcher_;
};

/** @} */ // addtogroup RpcReceiving

} // namespace srpc

#endif // SRPC_RPCEVENT_H
