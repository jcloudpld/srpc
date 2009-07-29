#include "stdafx.h"
#include <nsrpc/p2p/RpcPlugIn.h>
#include <nsrpc/p2p/P2pSession.h>
//#include <nsrpc/utility/Logger.h>

namespace nsrpc
{

IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcPlugIn);

RpcPlugIn::RpcPlugIn() :
    session_(0)
{
}


void RpcPlugIn::attached(P2pSession& session)
{
    session_ = &session;

    srpc::RpcReceiver::setRpcNetwork(session_->getRpcNetwork());
    srpc::RpcForwarder::setRpcNetwork(session_->getRpcNetwork());
}


void RpcPlugIn::detached()
{
    session_ = 0;

    srpc::RpcReceiver::resetRpcNetwork();
    srpc::RpcForwarder::resetRpcNetwork();
}

} // namespace nsrpc
