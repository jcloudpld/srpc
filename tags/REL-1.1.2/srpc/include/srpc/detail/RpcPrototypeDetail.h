#ifndef SRPC_RPCPROTOTYPEDETAIL_H
#define SRPC_RPCPROTOTYPEDETAIL_H

#include "../RpcPacketType.h"

/** @addtogroup RpcPrototype
* RPC �׽�Ʈ�� ��ũ�� ����
* @{
*/

// = SRPC_METHOD_PROTOTYPE_DETAIL_n

#define SRPC_METHOD_PROTOTYPE_DETAIL_0(rpcClass, method, rpcHint) \
    void rpcClass::method(const void* rpcHint)

#define SRPC_METHOD_PROTOTYPE_DETAIL_1(rpcClass, method, P1, p1, rpcHint) \
    void rpcClass::method(const P1& p1, const void* rpcHint)

#define SRPC_METHOD_PROTOTYPE_DETAIL_2(rpcClass, method, P1, p1, P2, p2, \
        rpcHint) \
    void rpcClass::method(const P1& p1, const P2& p2, const void* rpcHint)

#define SRPC_METHOD_PROTOTYPE_DETAIL_3(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, rpcHint) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const void* rpcHint)

#define SRPC_METHOD_PROTOTYPE_DETAIL_4(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, rpcHint) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const void* rpcHint)

#define SRPC_METHOD_PROTOTYPE_DETAIL_5(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, rpcHint) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const void* rpcHint)

#define SRPC_METHOD_PROTOTYPE_DETAIL_6(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, rpcHint) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const void* rpcHint)

#define SRPC_METHOD_PROTOTYPE_DETAIL_7(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, rpcHint) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const P7& p7, \
        const void* rpcHint)

/** @} */ // addtogroup RpcPrototype

#endif // SRPC_RPCPROTOTYPEDETAIL_H
