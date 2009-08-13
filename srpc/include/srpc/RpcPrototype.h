#ifndef SRPC_RPCPROTOTYPE_H
#define SRPC_RPCPROTOTYPE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "detail/RpcPrototypeDetail.h"

/** @addtogroup RpcPrototype
* RPC �׽�Ʈ�� ��ũ�� ����
* @{
*/

// = DEFINE_SRPC_METHOD_n

/// ��Ʈ�� ���� �Ķ���Ͱ� ���� RPC Method Prototype�� �����Ѵ�.
#define DEFINE_SRPC_METHOD_0(RpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 1���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_1(RpcClass, method, P1, p1) \
    DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, P1, p1, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 2���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_2(RpcClass, method, P1, p1, P2, p2) \
    DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, P1, p1, P2, p2, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 3���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_3(RpcClass, method, P1, p1, P2, p2, P3, p3) \
    DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, P1, p1, P2, p2, P3, p3, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 4���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_4(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4) \
    DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 5���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_5(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5) \
    DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 6���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_6(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6) \
    DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 7���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_7(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7) \
    DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7, )

// = DEFINE_SRPC_METHOD_WITH_HINT_n

/// ��Ʈ�� �ְ� �Ķ���Ͱ� ���� RPC Method Prototype�� �����Ѵ�.
#define DEFINE_SRPC_METHOD_WITH_HINT_0(RpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 1���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_WITH_HINT_1(RpcClass, method, P1, p1) \
    DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, P1, p1, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 2���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_WITH_HINT_2(RpcClass, method, P1, p1, P2, p2) \
    DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, P1, p1, P2, p2, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 3���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_WITH_HINT_3(RpcClass, method, P1, p1, P2, p2, \
        P3, p3) \
    DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 4���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_WITH_HINT_4(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4) \
    DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 5���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_WITH_HINT_5(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5) \
    DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 6���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_WITH_HINT_6(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6) \
    DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 7���� RPC Method Prototype�� �����Ѵ�
#define DEFINE_SRPC_METHOD_WITH_HINT_7(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7) \
    DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7, rpcHint)

/** @} */ // addtogroup RpcPrototype

#endif // SRPC_RPCPROTOTYPE_H
