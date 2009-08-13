#ifndef NSRPC_RPCPLUGIN_H
#define NSRPC_RPCPLUGIN_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "PlugIn.h"
#include "../nsrpc.h"
#include <srpc/RpcP2p.h>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @class RpcPlugIn
 * Plug-in abstract class for RPC
 */
class NSRPC_API RpcPlugIn : public PlugIn,
    protected srpc::RpcForwarder, private srpc::RpcReceiver
{
    DECLARE_SRPC_EVENT_DISPATCHER(RpcPlugIn);
public:
    RpcPlugIn();

    virtual void attached(P2pSession& session);

    virtual void detached();

private:
    P2pSession* session_;
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_RPCPLUGIN_H
