#ifndef SRPC_RPCFORWARDERDETAIL_H
#define SRPC_RPCFORWARDERDETAIL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "RpcCommand.h"
#include "ForwardingFunctors.h"
#include "../RpcPrototype.h"
#include "../RpcPacketType.h"

/** @addtogroup RpcForwarding
* RPC �۽�
* @{
*/

// = Rpc request Macros

// = FORWARD_SRPC_METHOD_DETAIL_n

/// �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�.
#define FORWARD_SRPC_METHOD_DETAIL_0(RpcClass, method, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_0(RpcClass, method) \
    { \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_0() > marshaler; \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// �Ķ���Ͱ� 1���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        P1, p1, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_1(RpcClass, method, P1, p1) \
    { \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_1(P1) > marshaler(p1); \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// �Ķ���Ͱ� 2���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        P1, p1, P2, p2, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_2(RpcClass, method, P1, p1, P2, p2) \
    { \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_2(P1, P2) > marshaler(p1, p2); \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// �Ķ���Ͱ� 3���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_3(RpcClass, method, P1, p1, P2, p2, \
        P3, p3) \
    { \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_3(P1, P2, P3) > \
            marshaler(p1, p2, p3); \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// �Ķ���Ͱ� 4���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_4(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4) \
    { \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_4(P1, P2, P3, P4) > \
            marshaler(p1, p2, p3, p4); \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// �Ķ���Ͱ� 5���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_5(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5) \
    { \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_5(P1, P2, P3, P4, P5) > \
            marshaler(p1, p2, p3, p4, p5); \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// �Ķ���Ͱ� 6���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_6(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6) \
    { \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6) > \
            marshaler(p1, p2, p3, p4, p5,  p6); \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/// �Ķ���Ͱ� 7���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, packetType) \
    DEFINE_SRPC_METHOD_WITH_HINT_7(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7) \
    { \
        srpc::ForwardingFunctorT< \
            SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7) > \
                marshaler(p1, p2, p3, p4, p5, p6, p7); \
        srpc::RpcCommand command( \
            RpcClass::SRPC_GET_RPCID(method)(), marshaler); \
        forward(command, packetType, rpcHint); \
    }

/** @} */ // addtogroup RpcForwarding

#endif // SRPC_RPCFORWARDERDETAIL_H
