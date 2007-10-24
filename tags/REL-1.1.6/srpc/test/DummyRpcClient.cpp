#include "stdafx.h"
#include "DummyRpcClient.h"

IMPLEMENT_SRPC_EVENT_DISPATCHER(DummyRpcClient);

DummyRpcClient::DummyRpcClient(srpc::RpcNetwork* rpcNetwork) :
    srpc::RpcReceiver(rpcNetwork),
    srpc::RpcForwarder(rpcNetwork)
{
}


DEFINE_SRPC_METHOD_0(DummyRpcClient, rpc0);
DEFINE_SRPC_METHOD_1(DummyRpcClient, rpc1, RInt32, p1);
DEFINE_SRPC_METHOD_2(DummyRpcClient, rpc2, RInt32, p1, RInt32, p2);
DEFINE_SRPC_METHOD_3(DummyRpcClient, rpc3, RInt32, p1, RInt32, p2,
    RInt32, p3);
DEFINE_SRPC_METHOD_4(DummyRpcClient, rpc4, RInt32, p1, RInt32, p2,
    RInt32, p3, RInt32, p4);
DEFINE_SRPC_METHOD_5(DummyRpcClient, rpc5, RInt32, p1, RInt32, p2,
    RInt32, p3, RInt32, p4, RInt32, p5);
DEFINE_SRPC_METHOD_6(DummyRpcClient, rpc6, RInt32, p1, RInt32, p2,
    RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6);
DEFINE_SRPC_METHOD_7(DummyRpcClient, rpc7, RInt32, p1, RInt32, p2,
    RInt32, p3, RInt32, p4, RInt32, p5, RInt32, p6, RInt32, p7);
DEFINE_SRPC_METHOD_2(DummyRpcClient, rpcBits, RInt15, p1, RInt31, p2);
DEFINE_SRPC_METHOD_0(DummyRpcClient, rpcFailed);

