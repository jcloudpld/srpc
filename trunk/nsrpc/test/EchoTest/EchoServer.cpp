#include "stdafx.h"
#include "EchoServer.h"

#if defined(NSRPC_HAS_PROACTOR)

IMPLEMENT_SRPC_EVENT_DISPATCHER(EchoServerRpcSession);

EchoServerRpcSession::EchoServerRpcSession(const nsrpc::RpcSessionConfig& config) :
    nsrpc::RpcProactorSession(config)
{
    registerRpcForwarder(*this);
    registerRpcReceiver(*this);
}


RECEIVE_SRPC_METHOD_1(EchoServerRpcSession, echo, srpc::RString, data)
{
    onEcho(data);
}


FORWARD_SRPC_METHOD_1(EchoServerRpcSession, onEcho, srpc::RString, data)

#endif // #if defined(NSRPC_HAS_PROACTOR)
