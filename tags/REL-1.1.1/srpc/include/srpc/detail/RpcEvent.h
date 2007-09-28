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
    RpcEvent(ReceivingFunctor& functor) :
        unmarshalDispatcher_(functor) {}

    template <class RpcClass>
    void dispatch(RpcClass& rpcClass, IStream& istream,
        const void* rpcHint = 0) {
        unmarshalDispatcher_(istream);
        unmarshalDispatcher_.call(&rpcClass, rpcHint);
    }
public:
    virtual RpcEvent* clone() const = 0;
private:
    ReceivingFunctor& unmarshalDispatcher_;
};

/** @} */ // addtogroup RpcReceiving

} // namespace srpc

#endif // SRPC_RPCEVENT_H
