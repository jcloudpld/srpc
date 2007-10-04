#include "stdafx.h"
#include "NamingServiceClient.h"

namespace ns {

IMPLEMENT_SRPC_EVENT_DISPATCHER(NamingServiceClient);

NamingServiceClient::NamingServiceClient(srpc::RpcNetwork* rpcNetwork) :
    srpc::RpcReceiver(rpcNetwork),
    srpc::RpcForwarder(rpcNetwork)
{
    reset();
}

// = 요청

DEFINE_SRPC_METHOD_2(NamingServiceClient, bind, srpc::RString, name,
    srpc::RString, value);
DEFINE_SRPC_METHOD_2(NamingServiceClient, rebind, srpc::RString, name,
    srpc::RString, value);
DEFINE_SRPC_METHOD_1(NamingServiceClient, unbind, srpc::RString, name);
DEFINE_SRPC_METHOD_1(NamingServiceClient, resolve, srpc::RString, name);

// = 응답


IMPLEMENT_SRPC_METHOD_3(NamingServiceClient, onBind,
    srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded)
{
    reset();
    if (succeeded) {
        name_ = name;
        value_ = value;
        succeeded_ = succeeded;
    }
}


IMPLEMENT_SRPC_METHOD_3(NamingServiceClient, onRebind,
    srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded)
{
    reset();
    if (succeeded) {
        name_ = name;
        value_ = value;
        succeeded_ = succeeded;
    }
}


IMPLEMENT_SRPC_METHOD_2(NamingServiceClient, onUnbind,
    srpc::RString, name, srpc::RBool, succeeded)
{
    reset();
    if (succeeded) {
        name_ = name;
        succeeded_ = succeeded;
    }
}


IMPLEMENT_SRPC_METHOD_3(NamingServiceClient, onResolve,
    srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded)
{
    reset();
    if (succeeded) {
        name_ = name;
        value_ = value;
        succeeded_ = succeeded;
    }
}


void NamingServiceClient::reset()
{
    name_.clear();
    value_.clear();
    succeeded_ = false;
}

} // namespace ns
