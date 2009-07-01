#include "stdafx.h"
#include "NamingServiceServer.h"

namespace ns {


IMPLEMENT_SRPC_EVENT_DISPATCHER(NamingServiceServer);

NamingServiceServer::NamingServiceServer(srpc::RpcNetwork* rpcNetwork) :
    srpc::RpcReceiver(rpcNetwork),
    srpc::RpcForwarder(rpcNetwork)
{
}

// = 처리

RECEIVE_SRPC_METHOD_2(NamingServiceServer, bind, srpc::RString, name,
    srpc::RString, value)
{
    bool succeeded =
        nameValues_.insert(NameValues::value_type(name, value)).second;
    onBind(name, value, succeeded);
}


RECEIVE_SRPC_METHOD_2(NamingServiceServer, rebind,
    srpc::RString, name, srpc::RString, value)
{
    bool succeeded = false;
    NameValues::iterator pos = nameValues_.find(name);
    if (pos != nameValues_.end()) {
        (*pos).second = value;
        succeeded = true;
    }
    else {
        succeeded = nameValues_.insert(
            NameValues::value_type(name, value)).second;
    }
    onRebind(name, value, succeeded);
}


RECEIVE_SRPC_METHOD_1(NamingServiceServer, unbind,
    srpc::RString, name)
{
    bool exists = false;
    NameValues::iterator pos = nameValues_.find(name);
    if (pos != nameValues_.end()) {
        nameValues_.erase(pos);
        exists = true;
    }
    onUnbind(name, exists);
}


RECEIVE_SRPC_METHOD_1(NamingServiceServer, resolve,
    srpc::RString, name)
{
    bool exists = false;
    srpc::RString value;
    NameValues::iterator pos = nameValues_.find(name);
    if (pos != nameValues_.end()) {
        value = (*pos).second;
        exists = true;
    }
    onResolve(name, value, exists);
}

// = 응답

FORWARD_SRPC_METHOD_3(NamingServiceServer, onBind, srpc::RString, name,
    srpc::RString, value, srpc::RBool, succeeded);
FORWARD_SRPC_METHOD_3(NamingServiceServer, onRebind, srpc::RString, name,
    srpc::RString, value, srpc::RBool, succeeded);
FORWARD_SRPC_METHOD_2(NamingServiceServer, onUnbind, srpc::RString, name,
    srpc::RBool, succeeded);
FORWARD_SRPC_METHOD_3(NamingServiceServer, onResolve,
    srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded);

} // namespace ns
