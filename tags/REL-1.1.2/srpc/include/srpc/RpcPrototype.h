#ifndef SRPC_RPCPROTOTYPE_H
#define SRPC_RPCPROTOTYPE_H

#include "detail/RpcPrototypeDetail.h"

/** @addtogroup RpcPrototype
* RPC �׽�Ʈ�� ��ũ�� ����
* @{
*/

// = SRPC_METHOD_PROTOTYPE_n

/// ��Ʈ�� ���� �Ķ���Ͱ� ���� RPC Method Prototype�� �����Ѵ�.
#define SRPC_METHOD_PROTOTYPE_0(rpcClass, method) \
    SRPC_METHOD_PROTOTYPE_DETAIL_0(rpcClass, method, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 1���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_1(rpcClass, method, P1, p1) \
    SRPC_METHOD_PROTOTYPE_DETAIL_1(rpcClass, method, P1, p1, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 2���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_2(rpcClass, method, P1, p1, P2, p2) \
    SRPC_METHOD_PROTOTYPE_DETAIL_2(rpcClass, method, P1, p1, P2, p2, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 3���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_3(rpcClass, method, P1, p1, P2, p2, P3, p3) \
    SRPC_METHOD_PROTOTYPE_DETAIL_3(rpcClass, method, P1, p1, P2, p2, P3, p3, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 4���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_4(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4) \
    SRPC_METHOD_PROTOTYPE_DETAIL_4(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 5���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_5(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5) \
    SRPC_METHOD_PROTOTYPE_DETAIL_5(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 6���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_6(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6) \
    SRPC_METHOD_PROTOTYPE_DETAIL_6(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 7���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_7(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7) \
    SRPC_METHOD_PROTOTYPE_DETAIL_7(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7, )

// = SRPC_METHOD_PROTOTYPE_WITH_HINT_n

/// ��Ʈ�� �ְ� �Ķ���Ͱ� ���� RPC Method Prototype�� �����Ѵ�.
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_0(rpcClass, method) \
    SRPC_METHOD_PROTOTYPE_DETAIL_0(rpcClass, method, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 1���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_1(rpcClass, method, P1, p1) \
    SRPC_METHOD_PROTOTYPE_DETAIL_1(rpcClass, method, P1, p1, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 2���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_2(rpcClass, method, P1, p1, P2, p2) \
    SRPC_METHOD_PROTOTYPE_DETAIL_2(rpcClass, method, P1, p1, P2, p2, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 3���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_3(rpcClass, method, P1, p1, P2, p2, \
        P3, p3) \
    SRPC_METHOD_PROTOTYPE_DETAIL_3(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 4���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_4(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4) \
    SRPC_METHOD_PROTOTYPE_DETAIL_4(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 5���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_5(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5) \
    SRPC_METHOD_PROTOTYPE_DETAIL_5(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 6���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_6(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6) \
    SRPC_METHOD_PROTOTYPE_DETAIL_6(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 7���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_7(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7) \
    SRPC_METHOD_PROTOTYPE_DETAIL_7(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7, rpcHint)

/** @} */ // addtogroup RpcPrototype

#endif // SRPC_RPCPROTOTYPE_H
