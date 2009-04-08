#ifndef SRPC_DUMMYRPCCLIENT_H
#define SRPC_DUMMYRPCCLIENT_H

#include "DummyRpcInterface.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>

/**
 * @class DummyRpcClient
 */
class DummyRpcClient : public DummyRpc,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(DummyRpcClient);
public:
    DummyRpcClient(srpc::RpcNetwork* rpcNetwork = 0);

    const srpc::RRpcId getLastRpcId() const {
        return lastRpcId_;
    }
private: // encapsulation
    DECLARE_SRPC_METHOD_0(rpc0);
    DECLARE_SRPC_METHOD_1(rpc1, RInt32, p1);
    DECLARE_SRPC_METHOD_2(rpc2, RInt32, p1, RInt32, p2);
    DECLARE_SRPC_METHOD_3(rpc3, RInt32, p1, RInt32, p2, RInt32, p3);
    DECLARE_SRPC_METHOD_4(rpc4, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4);
    DECLARE_SRPC_METHOD_5(rpc5, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5);
    DECLARE_SRPC_METHOD_6(rpc6, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5, RInt32, p6);
    DECLARE_SRPC_METHOD_7(rpc7, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5, RInt32, p6, RInt32, p7);
    DECLARE_SRPC_METHOD_2(rpcBits, RInt15, p1, RInt31, p2);
    DECLARE_SRPC_METHOD_0(rpcFailed);
private:
    virtual void onForwarding(const srpc::RRpcId& rpcId);
private:
    srpc::RRpcId lastRpcId_;
};

#endif // SRPC_DUMMYRPCCLIENT_H
