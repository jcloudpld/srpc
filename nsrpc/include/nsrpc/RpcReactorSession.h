#if !defined(NSRPC_RPCREACTORSESSION_H)
#define NSRPC_RPCREACTORSESSION_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "ReactorSession.h"
#include "PacketSeedExchangerCallback.h"
#include "detail/SessionRpcNetworkCallback.h"
#include <srpc/srpc_macros.h>
#include <boost/scoped_ptr.hpp>

namespace srpc
{

class RpcForwarder;
class RpcReceiver;

} // namespace srpc

namespace nsrpc
{

class SessionRpcNetwork;
class PacketSeedExchanger;

/**
 * @class RpcReactorSession
 * RPC ReactorSession
 * - 현재는 클라이언트용 세션만 지원한다.
 */
class NSRPC_API RpcReactorSession : public ReactorSession,
    private SessionRpcNetworkCallback,
    protected PacketSeedExchangerCallback
{
public:
    explicit RpcReactorSession(ACE_Reactor* reactor = 0,
        PacketCoderFactory* packetCoderFactory = 0,
        bool useBitPacking = true,
        bool shouldUseUtf8ForString = true);
    virtual ~RpcReactorSession();

    void registerRpcForwarder(srpc::RpcForwarder& forwarder);
    void registerRpcReceiver(srpc::RpcReceiver& receiver);

private:
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

#endif // !defined(NSRPC_RPCREACTORSESSION_H)
