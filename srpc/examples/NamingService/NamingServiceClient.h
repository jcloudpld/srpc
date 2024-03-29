#ifndef SRPC_NAMINGSERVICECLIENT_H
#define SRPC_NAMINGSERVICECLIENT_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "NamingService.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>

namespace ns {

/// Naming Service RPC Client
class NamingServiceClient : public NamingService,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(NamingServiceClient);
public:
    NamingServiceClient(srpc::RpcNetwork* rpcNetwork = 0);

    // = ��û
    OVERRIDE_SRPC_METHOD_2(bind, srpc::RString, name, srpc::RString, value);
    OVERRIDE_SRPC_METHOD_2(rebind, srpc::RString, name, srpc::RString, value);
    OVERRIDE_SRPC_METHOD_1(unbind, srpc::RString, name);
    OVERRIDE_SRPC_METHOD_1(resolve, srpc::RString, name);

    // = ����
    OVERRIDE_SRPC_METHOD_3(onBind, srpc::RString, name, srpc::RString, value,
        srpc::RBool, succeeded);
    OVERRIDE_SRPC_METHOD_3(onRebind, srpc::RString, name, srpc::RString, value,
        srpc::RBool, succeeded);
    OVERRIDE_SRPC_METHOD_2(onUnbind, srpc::RString, name,
        srpc::RBool, succeeded);
    OVERRIDE_SRPC_METHOD_3(onResolve, srpc::RString, name, srpc::RString, value,
        srpc::RBool, succeeded);

    const srpc::String& getName() const {
        return name_;
    }
    const srpc::String& getValue() const {
        return value_;
    }
    bool isSucceeded() const {
        return succeeded_;
    }
private:
    void reset();
private:
    srpc::String name_;
    srpc::String value_;
    bool succeeded_;
};

} /// namespace ns

#endif // SRPC_NAMINGSERVICECLIENT_H
