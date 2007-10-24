#ifndef SRPC_RPCPACKETTYPE_H
#define SRPC_RPCPACKETTYPE_H

#include "RpcTypes.h"

namespace srpc {

/// ��Ŷ�� ��� ������ ���ΰ�? (TCP/IP���� �ǹ̰� ����)
enum RpcPacketType
{
    ptReliable, ///< ���� ����. ���� ����
    ptUnreliable ///< ����, ���� �������� ����
};

/// �ŷڼ� �ִ� ��Ŷ�ΰ�?
inline bool isReliable(RpcPacketType packetType)
{
    return ptReliable == packetType;
}


/// ��ŷڼ� �ִ� ��Ŷ�ΰ�?
inline bool isUnreliable(RpcPacketType packetType)
{
    return ptUnreliable == packetType;
}


/**
 * RpcType for RpcPacketType
 * @warning enum�� int�� ��޵ǹǷ� �ݵ�� 1��Ʈ(��ȣ)�� �� �߰��ؾ� �Ѵ�.
 */
typedef RpcUIntType<RpcPacketType, 1 + 1> RRpcPacketType;

} // namespace srpc

#endif // SRPC_RPCPACKETTYPE_H
