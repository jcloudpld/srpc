#ifndef NSRPC_RPCSESSION_H
#define NSRPC_RPCSESSION_H

#include "Session.h"
#include "detail/SessionRpcNetworkCallback.h"
#include "detail/rpc_interface/SecurityService.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>
#include <boost/scoped_ptr.hpp>

namespace srpc
{

class RpcNetwork;

} // namespace srpc

namespace nsrpc
{

class SessionRpcNetwork;
class PacketSeedExchanger;
struct RpcSessionConfig;

/**
 * @class RpcSession
 *
 * RPC enabled Session
 */
class NSRPC_API RpcSession : public Session,
    protected srpc::RpcReceiver, protected srpc::RpcForwarder,
    private SessionRpcNetworkCallback
{
    DECLARE_SRPC_EVENT_DISPATCHER(RpcSession);
public:
    RpcSession(const RpcSessionConfig& config);
    virtual ~RpcSession();
public: // for Test
    srpc::RpcNetwork* getRpcNetwork();
public:
    // = NSRPC_Service_Handler
    virtual void open(ACE_HANDLE new_handle, ACE_Message_Block& message_block);
protected:
    // = Session overriding
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
private:
    boost::scoped_ptr<SessionRpcNetwork> rpcNetwork_;
    boost::scoped_ptr<PacketSeedExchanger> seedExchanger_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_RPCSESSION_H)
