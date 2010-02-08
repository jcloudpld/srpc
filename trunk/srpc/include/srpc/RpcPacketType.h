#ifndef SRPC_RPCPACKETTYPE_H
#define SRPC_RPCPACKETTYPE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "RpcTypes.h"

namespace srpc {

/// ��Ŷ�� ��� ������ ���ΰ�? (TCP/IP���� �ǹ̰� ����)
enum RpcPacketType
{
    ptUnknown = -1,
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
 */
typedef RpcNumericType<RpcPacketType, 1, UInt8> RRpcPacketType;

} // namespace srpc

#endif // SRPC_RPCPACKETTYPE_H
