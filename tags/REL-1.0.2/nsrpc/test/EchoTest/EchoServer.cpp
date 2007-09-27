#include "stdafx.h"
#include "EchoServer.h"

IMPLEMENT_SRPC_METHOD_1(EchoServerRpcSession, echo, srpc::RString, data)
{
    onEcho(data);
}


DEFINE_SRPC_METHOD_1(EchoServerRpcSession, onEcho, srpc::RString, data)
