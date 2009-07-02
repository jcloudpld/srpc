#include "stdafx.h"
#include "EchoClient.h"

#if defined(NSRPC_HAS_PROACTOR)

#include <srpc/Exception.h>

IMPLEMENT_SRPC_EVENT_DISPATCHER(EchoClientRpcSession);

EchoClientRpcSession::EchoClientRpcSession(srpc::UInt32 echoCount, srpc::UInt32 blockSize,
    const nsrpc::RpcSessionConfig& config) :
    nsrpc::RpcProactorSession(config),
    echoCount_(echoCount),
    blockSize_(blockSize),
    body_(blockSize - getPacketCoder().getHeaderSize(), 'R')
{
    registerRpcForwarder(*this);
    registerRpcReceiver(*this);
}


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
