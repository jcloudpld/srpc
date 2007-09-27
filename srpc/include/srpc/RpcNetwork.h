#ifndef SRPC_RPCNETWORK_H
#define SRPC_RPCNETWORK_H

#include <srpc/config/config.h>
#include <srpc/RpcPacketType.h>
#include <srpc/ContainerTypes.h>
#include <srpc/detail/RpcId.h>
#include <boost/utility.hpp>
#include <cassert>

namespace srpc {

class RpcReceiver;
class IStream;
class RpcCommand;

/**
 * @class RpcNetwork
 *
 * rpc network
 */
class SRPC_API RpcNetwork : public boost::noncopyable
{
    typedef Vector<RpcReceiver*> RpcReceivers;
public:
    RpcNetwork();
    virtual ~RpcNetwork() {}

    /**
     * RpReceiver를 등록한다.
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다.
     */
    virtual void registerRpcReceiver(RpcReceiver* receiver);

    /**
     * RpReceiver의 등록을 해제한다
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다.
     */
    virtual void unregisterRpcReceiver(RpcReceiver* receiver);

    /**
     * 서버로 메세지를 전송한다.
     * @param command RPC Command 인스턴스.
     * @param packetType 패킷을 어떻게 전송할 것인가?
     * @param rpcHint RPC method를 통해 전달된 힌트 포인터.
     * - RpcCommand::marshal()을 호출하여 스트림으로 변환한 후 전송해야 한다.
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다.
     */
    virtual void send(RpcCommand& command,
        RpcPacketType packetType = ptReliable,
        const void* rpcHint = 0) = 0;

    /**
     * 서버로 부터 메세지가 도착하였다
     * @param istream 입력 스트림.
     * @param rpcHint RPC method로 전달될 힌트 포인터.
     * @exception StreamingException 스트리밍 에러가 발생할 경우
     * @exception UnknownRpcMethodException 알 수 없는 요청이 발생할 경우
     * @exception RpcFailedException RPC요청을 처리하는 도중 에러가 발생할 경우
     * - 다중 쓰레드 환경 하에서는 배타적(thread-safe)으로 처리해야 한다
     */
    virtual void onReceive(IStream& istream, const void* rpcHint = 0);
private:
    RpcReceivers receivers_;
};

} // namespace srpc

#endif // SRPC_RPCNETWORK_H
