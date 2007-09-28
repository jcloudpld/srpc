#ifndef SRPC_RPCPROTOTYPE_H
#define SRPC_RPCPROTOTYPE_H

#include "detail/RpcPrototypeDetail.h"

/** @addtogroup RpcPrototype
* RPC 테스트용 매크로 정의
* @{
*/

// = SRPC_METHOD_PROTOTYPE_n

/// 힌트가 없고 파라미터가 없는 RPC Method Prototype을 정의한다.
#define SRPC_METHOD_PROTOTYPE_0(rpcClass, method) \
    SRPC_METHOD_PROTOTYPE_DETAIL_0(rpcClass, method, )

/// 힌트가 없고 파라미터가 1개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_1(rpcClass, method, P1, p1) \
    SRPC_METHOD_PROTOTYPE_DETAIL_1(rpcClass, method, P1, p1, )

/// 힌트가 없고 파라미터가 2개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_2(rpcClass, method, P1, p1, P2, p2) \
    SRPC_METHOD_PROTOTYPE_DETAIL_2(rpcClass, method, P1, p1, P2, p2, )

/// 힌트가 없고 파라미터가 3개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_3(rpcClass, method, P1, p1, P2, p2, P3, p3) \
    SRPC_METHOD_PROTOTYPE_DETAIL_3(rpcClass, method, P1, p1, P2, p2, P3, p3, )

/// 힌트가 없고 파라미터가 4개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_4(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4) \
    SRPC_METHOD_PROTOTYPE_DETAIL_4(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, )

/// 힌트가 없고 파라미터가 5개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_5(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5) \
    SRPC_METHOD_PROTOTYPE_DETAIL_5(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, )

/// 힌트가 없고 파라미터가 6개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_6(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6) \
    SRPC_METHOD_PROTOTYPE_DETAIL_6(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, )

/// 힌트가 없고 파라미터가 7개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_7(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7) \
    SRPC_METHOD_PROTOTYPE_DETAIL_7(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7, )

// = SRPC_METHOD_PROTOTYPE_WITH_HINT_n

/// 힌트가 있고 파라미터가 없는 RPC Method Prototype을 정의한다.
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_0(rpcClass, method) \
    SRPC_METHOD_PROTOTYPE_DETAIL_0(rpcClass, method, rpcHint)

/// 힌트가 있고 파라미터가 1개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_1(rpcClass, method, P1, p1) \
    SRPC_METHOD_PROTOTYPE_DETAIL_1(rpcClass, method, P1, p1, rpcHint)

/// 힌트가 있고 파라미터가 2개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_2(rpcClass, method, P1, p1, P2, p2) \
    SRPC_METHOD_PROTOTYPE_DETAIL_2(rpcClass, method, P1, p1, P2, p2, rpcHint)

/// 힌트가 있고 파라미터가 3개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_3(rpcClass, method, P1, p1, P2, p2, \
        P3, p3) \
    SRPC_METHOD_PROTOTYPE_DETAIL_3(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        rpcHint)

/// 힌트가 있고 파라미터가 4개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_4(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4) \
    SRPC_METHOD_PROTOTYPE_DETAIL_4(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, rpcHint)

/// 힌트가 있고 파라미터가 5개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_5(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5) \
    SRPC_METHOD_PROTOTYPE_DETAIL_5(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, rpcHint)

/// 힌트가 있고 파라미터가 6개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_6(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6) \
    SRPC_METHOD_PROTOTYPE_DETAIL_6(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, rpcHint)

/// 힌트가 있고 파라미터가 7개인 RPC Method Prototype을 정의한다
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_7(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7) \
    SRPC_METHOD_PROTOTYPE_DETAIL_7(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7, rpcHint)

/** @} */ // addtogroup RpcPrototype

#endif // SRPC_RPCPROTOTYPE_H
