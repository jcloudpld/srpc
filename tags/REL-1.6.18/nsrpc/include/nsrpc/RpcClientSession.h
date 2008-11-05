#if !defined(NSRPC_RPCCLIENTSESSION_H)
#define NSRPC_RPCCLIENTSESSION_H

#include "detail/ClientSession.h"
#include "detail/SessionRpcNetworkCallback.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>
#include <boost/scoped_ptr.hpp>

namespace nsrpc
{

class SessionRpcNetwork;
class PacketSeedExchanger;

/**
 * @class RpcClientSession
 * RPC Client Session
 */
class NSRPC_API RpcClientSession : public ClientSession,
    protected srpc::RpcReceiver, protected srpc::RpcForwarder,
    private SessionRpcNetworkCallback
{
    DECLARE_SRPC_EVENT_DISPATCHER(RpcClientSession);
public:
    RpcClientSession(ACE_Reactor* reactor,
        PacketCoderFactory* packetCoderFactory = 0,
        bool useBitPacking = true);
    virtual ~RpcClientSession();

    SessionRpcNetwork* getRpcNetwork();
private:
    void initRpcNetwork();
    void sendingFailed();
    void receivingFailed();
protected:
    virtual void onConnected();
    virtual void onDisconnected();
    virtual void onMessageArrived(CsMessageType messageType);
private:
    /// 전송 중 에러가 발생하였다(marshaling 실패 등)
    virtual void onSendError() = 0;

    /// 수신 중 에러가 발생하였다(unmarshaling 실패 등)
    virtual void onReceiveError() = 0;
private:
    // = SessionRpcNetworkCallback overriding
    virtual void sendNow(ACE_Message_Block& mblock,
        CsMessageType messageType) {
        sendMessage(mblock, messageType);
    }
    virtual void marshalingErrorOccurred() {
        sendingFailed();
    }
    virtual void unmarshalingErrorOccurred() {
        receivingFailed();
    }
private:
    boost::scoped_ptr<SessionRpcNetwork> rpcNetwork_;
    boost::scoped_ptr<PacketSeedExchanger> seedExchanger_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_RPCCLIENTSESSION_H)
