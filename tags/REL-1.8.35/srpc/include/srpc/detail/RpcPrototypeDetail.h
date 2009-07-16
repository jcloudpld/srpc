#ifndef SRPC_RPCPROTOTYPEDETAIL_H
#define SRPC_RPCPROTOTYPEDETAIL_H

#include "../RpcPacketType.h"

/** @addtogroup RpcPrototype
* RPC �׽�Ʈ�� ��ũ�� ����
* @{
*/

// = DEFINE_SRPC_METHOD_DETAIL_n

#define DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint) \
    void RpcClass::method(const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, P1, p1, rpcHint) \
    void RpcClass::method(const P1& p1, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, P1, p1, P2, p2, \
        rpcHint) \
    void RpcClass::method(const P1& p1, const P2& p2, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, rpcHint) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, rpcHint) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, rpcHint) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, rpcHint) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const void* rpcHint)

#define DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, rpcHint) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const P7& p7, \
        const void* rpcHint)

/** @} */ // addtogroup RpcPrototype

#endif // SRPC_RPCPROTOTYPEDETAIL_H
