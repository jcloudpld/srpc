#include "stdafx.h"
#include "EchoClient.h"

#if defined(NSRPC_HAS_PROACTOR)

#include <srpc/Exception.h>

FORWARD_SRPC_METHOD_1(EchoClientRpcSession, echo, srpc::RString, data)

RECEIVE_SRPC_METHOD_1(EchoClientRpcSession, onEcho,
    srpc::RString, data)
{
    assert(echoedCount_ < echoCount_);
    if ((++echoedCount_) == echoCount_) {
        //throw srpc::RpcFailedException(__FILE__, __LINE__, "finished");
        disconnect();
    }
    else {
        echo(data);
    }
}

#endif // #if defined(NSRPC_HAS_PROACTOR)
