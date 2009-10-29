#ifndef SRPC_NAMINGSERVICESERVER_H
#define SRPC_NAMINGSERVICESERVER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "NamingService.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>
#include <srpc/ContainerTypes.h>

namespace ns {

/// Naming Service RPC Server
class NamingServiceServer : public NamingService,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(NamingServiceServer);
public:
    NamingServiceServer(srpc::RpcNetwork* rpcNetwork = 0);

    // = 처리
    OVERRIDE_SRPC_METHOD_2(bind, srpc::RString, name, srpc::RString, value);
    OVERRIDE_SRPC_METHOD_2(rebind, srpc::RString, name, srpc::RString, value);
    OVERRIDE_SRPC_METHOD_1(unbind, srpc::RString, name);
    OVERRIDE_SRPC_METHOD_1(resolve, srpc::RString, name);

    // = 응답
private: // encapsulation
    OVERRIDE_SRPC_METHOD_3(onBind, srpc::RString, name, srpc::RString, value,
        srpc::RBool, succeeded);
    OVERRIDE_SRPC_METHOD_3(onRebind, srpc::RString, name, srpc::RString, value,
        srpc::RBool, succeeded);
    OVERRIDE_SRPC_METHOD_2(onUnbind, srpc::RString, name,
        srpc::RBool, succeeded);
    OVERRIDE_SRPC_METHOD_3(onResolve, srpc::RString, name, srpc::RString, value,
        srpc::RBool, succeeded);
private:
    typedef srpc::Map<srpc::String, srpc::String> NameValues;
    NameValues nameValues_;
};

} /// namespace ns

#endif // SRPC_NAMINGSERVICESERVER_H
