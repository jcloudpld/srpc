#ifndef ECHOTEST_ECHO_H
#define ECHOTEST_ECHO_H

#include <srpc/RpcInterface.h>
#include <srpc/RpcTypes.h>

class Echo
{
public:
    virtual ~Echo() {}

    DECLARE_SRPC_PURE_METHOD_1(Echo, echo, srpc::RString, data);

    DECLARE_SRPC_PURE_METHOD_1(Echo, onEcho, srpc::RString, data);
};

#endif // ECHOTEST_ECHO_H
