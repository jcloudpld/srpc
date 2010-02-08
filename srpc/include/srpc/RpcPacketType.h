#ifndef SRPC_RPCPACKETTYPE_H
#define SRPC_RPCPACKETTYPE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "RpcTypes.h"

namespace srpc {

/// 패킷을 어떻게 전송할 것인가? (TCP/IP에선 의미가 없다)
enum RpcPacketType
{
    ptUnknown = -1,
    ptReliable, ///< 순서 보장. 전송 보장
    ptUnreliable ///< 순서, 전송 보장하지 않음
};

/// 신뢰성 있는 패킷인가?
inline bool isReliable(RpcPacketType packetType)
{
    return ptReliable == packetType;
}


/// 비신뢰성 있는 패킷인가?
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
