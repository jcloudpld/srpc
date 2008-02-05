#ifndef SRPC_DUMMYRPCEVENT_H
#define SRPC_DUMMYRPCEVENT_H

#include <srpc/detail/RpcEvent.h>
#include <srpc/detail/ReceivingFunctors.h>
#include <srpc/RpcTypes.h>

/**
 * @class DummyRpcEvent
 */
class DummyRpcEvent : public srpc::RpcEvent
{
public:
    DummyRpcEvent() :
        srpc::RpcEvent(srpc::RRpcId()),
        unmarshalFunctor_(&DummyRpcEvent::handler2),
        p1_(0),
        p2_(0) {}

    srpc::Int32 getP1() const {
        return p1_;
    }
    srpc::Int32 getP2() const {
        return p2_;
    }
private:
    virtual srpc::RpcEvent* clone() const {
        return 0;
    }
    virtual srpc::ReceivingFunctor& getDispatcher() {
        return unmarshalFunctor_;
    }
private:
    void handler2(const srpc::RInt32& p1, const srpc::RInt32& p2,
        const void* /*rpcHint*/) {
        p1_ = p1;
        p2_ = p2;
    }
private:
    srpc::ReceivingFunctorT<DummyRpcEvent,
        SRPC_TYPELIST_2(srpc::RInt32, srpc::RInt32)> unmarshalFunctor_;
    srpc::RInt32 p1_;
    srpc::RInt32 p2_;
};

#endif // SRPC_DUMMYRPCEVENT_H
