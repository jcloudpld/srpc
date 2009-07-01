#ifndef SRPC_DUMMYRPCSERVER_H
#define SRPC_DUMMYRPCSERVER_H

#include "DummyRpcInterface.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>
#include <srpc/detail/RpcId.h>
#include <string>

/**
 * @class DummyRpcServer
 */
class DummyRpcServer : public DummyRpc,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(DummyRpcServer);
public:
    DummyRpcServer(srpc::RpcNetwork* rpcNetwork = 0);

    OVERRIDE_SRPC_METHOD_0(rpc0);
    OVERRIDE_SRPC_METHOD_1(rpc1, RInt32, p1);
    OVERRIDE_SRPC_METHOD_2(rpc2, RInt32, p1, RInt32, p2);
    OVERRIDE_SRPC_METHOD_3(rpc3, RInt32, p1, RInt32, p2, RInt32, p3);
    OVERRIDE_SRPC_METHOD_4(rpc4, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4);
    OVERRIDE_SRPC_METHOD_5(rpc5, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5);
    OVERRIDE_SRPC_METHOD_6(rpc6, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5, RInt32, p6);
    OVERRIDE_SRPC_METHOD_7(rpc7, RInt32, p1, RInt32, p2, RInt32, p3,
        RInt32, p4, RInt32, p5, RInt32, p6, RInt32, p7);
    OVERRIDE_SRPC_METHOD_2(rpcBits, RInt15, p1, RInt31, p2);
    OVERRIDE_SRPC_METHOD_0(rpcFailed);
private:
    virtual void onReceiving(const srpc::RRpcId& rpcId) {
        lastRpcId_ = rpcId;
    }
public:
    const srpc::RRpcId& getRpcId() const {
        return rpcId_;
    }
    srpc::Int32 getP1() const {
        return p1_;
    }
    srpc::Int32 getP2() const {
        return p2_;
    }
    srpc::Int32 getP3() const {
        return p3_;
    }
    srpc::Int32 getP4() const {
        return p4_;
    }
    srpc::Int32 getP5() const {
        return p5_;
    }
    srpc::Int32 getP6() const {
        return p6_;
    }
    srpc::Int32 getP7() const {
        return p7_;
    }
    srpc::Int32 getP8() const {
        return p8_;
    }
    srpc::Int32 getP9() const {
        return p9_;
    }
    srpc::Int32 getP10() const {
        return p10_;
    }
    int getHint() const {
        return rpcHint_;
    }

    const srpc::RRpcId& getLastRpcId() const {
        return lastRpcId_;
    }
private:
    srpc::RRpcId rpcId_;
    srpc::Int32 p1_;
    srpc::Int32 p2_;
    srpc::Int32 p3_;
    srpc::Int32 p4_;
    srpc::Int32 p5_;
    srpc::Int32 p6_;
    srpc::Int32 p7_;
    srpc::Int32 p8_;
    srpc::Int32 p9_;
    srpc::Int32 p10_;
    int rpcHint_;

    srpc::RRpcId lastRpcId_;
};


/**
 * @class DummyRpcServer2
 */
class DummyRpcServer2 : public DummyRpc2,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(DummyRpcServer2);
public:
    DummyRpcServer2(srpc::RpcNetwork* rpcNetwork = 0);

    OVERRIDE_SRPC_METHOD_1(rpc1, RInt32, p1);
public:
    const srpc::RpcId getRpcId() const {
        return rpcId_;
    }
    srpc::Int32 getP1() const {
        return p1_;
    }
private:
    srpc::RpcId rpcId_;
    srpc::Int32 p1_;
};

#endif // SRPC_DUMMYRPCSERVER_H
