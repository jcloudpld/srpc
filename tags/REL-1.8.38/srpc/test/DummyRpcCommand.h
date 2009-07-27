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
    DummyRpcCommand(const srpc::RInt32& p1, const srpc::RInt32& p2) :
      srpc::RpcCommand(DummyRpcCommand::getStaticRpcId()),
        marshalFunctor_(p1, p2) {}
private:
    virtual srpc::ForwardingFunctor& getMarshaler() {
        return marshalFunctor_;
    }

private:
    static const srpc::RRpcId& getStaticRpcId() {
        static srpc::RRpcId rpcId("DummyRpcCommand");
        return rpcId;
    }
private:
    srpc::ForwardingFunctorT<SRPC_TYPELIST_2(srpc::RInt32, srpc::RInt32)>
        marshalFunctor_;
};

#endif // SRPC_DUMMYRPCCOMMAND_H
