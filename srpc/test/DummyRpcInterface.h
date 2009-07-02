#ifndef SRPC_DUMMYINTERFACE_H
#define SRPC_DUMMYINTERFACE_H

#include <srpc/RpcInterface.h>
#include <srpc/RpcTypes.h>

using srpc::Int16;
using srpc::Int32;
using srpc::RInt32;
using srpc::RpcIntType;

typedef RpcIntType<Int16, 15> RInt15;
typedef RpcIntType<Int32, 31> RInt31;

/**
 * @class DummyRpc
 */
class DummyRpc
{
public:
    virtual ~DummyRpc() {}

    // = request

    DECLARE_SRPC_METHOD_0(DummyRpc, rpc0);
    DECLARE_SRPC_METHOD_1(DummyRpc, rpc1, RInt32, p1);
    DECLARE_SRPC_METHOD_2(DummyRpc, rpc2, RInt32, p1, RInt32, p2);
    DECLARE_SRPC_METHOD_3(DummyRpc, rpc3, RInt32, p1, RInt32, p2,
        RInt32, p3);
    DECLARE_SRPC_METHOD_4(DummyRpc, rpc4, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4);
    DECLARE_SRPC_METHOD_5(DummyRpc, rpc5, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4, RInt32, p5);
    DECLARE_SRPC_METHOD_6(DummyRpc, rpc6, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6);
    DECLARE_SRPC_METHOD_7(DummyRpc, rpc7, RInt32, p1, RInt32, p2,
        RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6, RInt32, p7);
    DECLARE_SRPC_METHOD_2(DummyRpc, rpcBits, RInt15, p1, RInt31, p2);
    DECLARE_SRPC_METHOD_0(DummyRpc, rpcFailed);
};


/**
 * @class DummyRpc2
 */
class DummyRpc2
{
public:
    virtual ~DummyRpc2() {}

    // = request
    DECLARE_SRPC_METHOD_1(DummyRpc2, rpc1, RInt32, p1);
};

#endif // SRPC_DUMMYINTERFACE_H
