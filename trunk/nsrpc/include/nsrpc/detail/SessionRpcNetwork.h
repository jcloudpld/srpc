#ifndef NSRPC_SESSIONRPCNETWORK_H
#define NSRPC_SESSIONRPCNETWORK_H

#include <nsrpc/detail/CsProtocol.h>
#include <nsrpc/nsrpc.h>
#include <srpc/RpcNetwork.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4267 4312 4996)
#endif
#include <ace/Thread_Mutex.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/scoped_ptr.hpp>

class ACE_Recursive_Thread_Mutex;
class ACE_Message_Block;

namespace srpc
{

class IStream;
class OStream;

} // namespace srpc

namespace nsrpc
{

class SessionRpcNetworkCallback;
class MessageBlockProvider;
class MessageBlockStreamBuffer;

/** @addtogroup session
* @{
*/

/**
 * @class SessionRpcNetwork
 *
 * Asynchronous Session RpcNetwork
 */
class NSRPC_API SessionRpcNetwork : public srpc::RpcNetwork
{
public:
    SessionRpcNetwork(bool useBitPacking);
    virtual ~SessionRpcNetwork();

    void initialize(SessionRpcNetworkCallback& callback,
        MessageBlockProvider& messageBlockProvider);

    /// 입력 스트림을 초기화한다
    void reset();

    /// input/output을 중지한다
    void disable() {
        enabled_ = false;
    }

    /// 상대방으로 부터 메세지가 도착하였다
    virtual bool messageArrived(CsMessageType messageType);
protected:
    /// 수신 메세지를 지금 바로 처리한다
    /// @param mblock 메모리 소유권을 넘기지 않는다
    bool handleMessageNow(ACE_Message_Block& mblock);

    void unmarshalingErrorOccurred();

    ACE_Message_Block* getRecvBlock();
public: // for Test
    virtual void registerRpcReceiver(srpc::RpcReceiver& receiver);
    virtual void unregisterRpcReceiver(srpc::RpcReceiver& receiver);
    virtual void send(srpc::RpcCommand& command,
        srpc::RpcPacketType packetType, const void* rpcHint);
private:
    ACE_Message_Block* marshal(srpc::RpcCommand& command);
    void initInputStream(ACE_Message_Block& mblock);
    ACE_Message_Block* initOutputStream();
private:
    SessionRpcNetworkCallback* callback_;
    MessageBlockProvider* messageBlockProvider_;

    bool enabled_;

    bool useBitPacking_;

    boost::scoped_ptr<MessageBlockStreamBuffer> rstreamBuffer_;
    boost::scoped_ptr<srpc::IStream> 
        istream_;

    boost::scoped_ptr<MessageBlockStreamBuffer> wstreamBuffer_;
    boost::scoped_ptr<srpc::OStream> ostream_;

    ACE_Thread_Mutex sendLock_;
    ACE_Thread_Mutex recvLock_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // !defined(NSRPC_SESSIONRPCNETWORK_H)
