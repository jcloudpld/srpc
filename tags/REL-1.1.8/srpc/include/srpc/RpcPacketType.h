#ifndef SRPC_RPCPACKETTYPE_H
#define SRPC_RPCPACKETTYPE_H

#include "RpcTypes.h"

namespace srpc {

/// 패킷을 어떻게 전송할 것인가? (TCP/IP에선 의미가 없다)
enum RpcPacketType
{
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
 * @warning enum은 int로 취급되므로 반드시 1비트(부호)를 더 추가해야 한다.
 */
typedef RpcUIntType<RpcPacketType, 1 + 1> RRpcPacketType;

} // namespace srpc

#endif // SRPC_RPCPACKETTYPE_H
