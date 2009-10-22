#ifndef NSRPC_RPCPROACTORSESSION_H
#define NSRPC_RPCPROACTORSESSION_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "nsrpc.h"
#include "config/Proactor.h"

#if defined (NSRPC_HAS_PROACTOR)

#include "ProactorSession.h"
#include "PacketSeedExchangerCallback.h"
#include "detail/SessionRpcNetworkCallback.h"
#include "detail/rpc_interface/SecurityService.h"
#include <srpc/srpc_macros.h>
#include <boost/scoped_ptr.hpp>

namespace srpc
{

class RpcForwarder;
class RpcReceiver;
class RpcNetwork;

} // namespace srpc

namespace nsrpc
{

class SessionRpcNetwork;
class PacketSeedExchanger;
struct RpcSessionConfig;

/**
 * @class RpcProactorSession
 *
 * RPC enabled Session
 */
class NSRPC_API RpcProactorSession : public ProactorSession,
    private SessionRpcNetworkCallback,
    protected PacketSeedExchangerCallback
{
public:
    RpcProactorSession(const RpcSessionConfig& config);
    virtual ~RpcProactorSession();

    void registerRpcForwarder(srpc::RpcForwarder& forwarder);
    void registerRpcReceiver(srpc::RpcReceiver& receiver);

protected:
    // = Session overriding
    virtual bool onConnected();
    virtual void onDisconnected();
    virtual bool onMessageArrived(CsMessageType messageType);
private:
    // = SessionRpcNetworkCallback overriding
    virtual void sendNow(ACE_Message_Block& mblock,
        CsMessageType messageType) {
        sendMessage(mblock, messageType);
    }
    virtual void marshalingErrorOccurred() {
        disconnect();
    }
    virtual void unmarshalingErrorOccurred() {
        disconnect();
    }

protected:
    srpc::RpcNetwork* getRpcNetwork();
private:
    boost::scoped_ptr<SessionRpcNetwork> rpcNetwork_;
    boost::scoped_ptr<PacketSeedExchanger> seedExchanger_;
};

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // !defined(NSRPC_RPCPROACTORSESSION_H)
