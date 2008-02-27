#ifndef SRPC_DUMMYRPCCOMMAND_H
#define SRPC_DUMMYRPCCOMMAND_H

#include <srpc/detail/RpcCommand.h>
#include <srpc/RpcTypes.h>
#include <srpc/detail/ForwardingFunctors.h>

/**
 * @class DummyRpcCommand
 */
class DummyRpcCommand : public srpc::RpcCommand
{
public:
    DummyRpcCommand(srpc::RInt32 p1, srpc::RInt32 p2) :
      srpc::RpcCommand(srpc::RRpcId("DummyRpcCommand")),
        marshalFunctor_(p1, p2) {}
private:
    virtual srpc::ForwardingFunctor& getMarshaler() {
        return marshalFunctor_;
    }
private:
    srpc::ForwardingFunctorT<SRPC_TYPELIST_2(srpc::RInt32, srpc::RInt32)>
        marshalFunctor_;
};

#endif // SRPC_DUMMYRPCCOMMAND_H
