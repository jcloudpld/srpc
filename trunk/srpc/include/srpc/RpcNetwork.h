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
     * RpReceiver�� ����Ѵ�.
     * - ���� ������ ȯ�� �Ͽ����� ��Ÿ��(thread-safe)���� ó���ؾ� �Ѵ�.
     */
    virtual void registerRpcReceiver(RpcReceiver* receiver);

    /**
     * RpReceiver�� ����� �����Ѵ�
     * - ���� ������ ȯ�� �Ͽ����� ��Ÿ��(thread-safe)���� ó���ؾ� �Ѵ�.
     */
    virtual void unregisterRpcReceiver(RpcReceiver* receiver);

    /**
     * ������ �޼����� �����Ѵ�.
     * @param command RPC Command �ν��Ͻ�.
     * @param packetType ��Ŷ�� ��� ������ ���ΰ�?
     * @param rpcHint RPC method�� ���� ���޵� ��Ʈ ������.
     * - RpcCommand::marshal()�� ȣ���Ͽ� ��Ʈ������ ��ȯ�� �� �����ؾ� �Ѵ�.
     * - ���� ������ ȯ�� �Ͽ����� ��Ÿ��(thread-safe)���� ó���ؾ� �Ѵ�.
     */
    virtual void send(RpcCommand& command,
        RpcPacketType packetType = ptReliable,
        const void* rpcHint = 0) = 0;

    /**
     * ������ ���� �޼����� �����Ͽ���
     * @param istream �Է� ��Ʈ��.
     * @param rpcHint RPC method�� ���޵� ��Ʈ ������.
     * @exception StreamingException ��Ʈ���� ������ �߻��� ���
     * @exception UnknownRpcMethodException �� �� ���� ��û�� �߻��� ���
     * @exception RpcFailedException RPC��û�� ó���ϴ� ���� ������ �߻��� ���
     * - ���� ������ ȯ�� �Ͽ����� ��Ÿ��(thread-safe)���� ó���ؾ� �Ѵ�
     */
    virtual void onReceive(IStream& istream, const void* rpcHint = 0);
private:
    RpcReceivers receivers_;
};

} // namespace srpc

#endif // SRPC_RPCNETWORK_H
