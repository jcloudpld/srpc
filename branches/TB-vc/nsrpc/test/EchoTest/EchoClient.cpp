#include "stdafx.h"
#include "EchoClient.h"
#include <srpc/Exception.h>

DEFINE_SRPC_METHOD_1(EchoClientRpcSession, echo, srpc::RString, data)

IMPLEMENT_SRPC_METHOD_1(EchoClientRpcSession, onEcho,
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
