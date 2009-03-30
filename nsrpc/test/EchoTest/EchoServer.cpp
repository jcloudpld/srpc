#include "stdafx.h"
#include "EchoServer.h"

#if defined(NSRPC_HAS_PROACTOR)

IMPLEMENT_SRPC_METHOD_1(EchoServerRpcSession, echo, srpc::RString, data)
{
    onEcho(data);
}


DEFINE_SRPC_METHOD_1(EchoServerRpcSession, onEcho, srpc::RString, data)

#endif // #if defined(NSRPC_HAS_PROACTOR)
