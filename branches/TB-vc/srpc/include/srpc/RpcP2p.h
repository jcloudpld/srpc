#ifndef SRPC_RPCP2P_H
#define SRPC_RPCP2P_H

#include "RpcReceiver.h"
#include "RpcForwarder.h"
#include "RpcInterface.h"

/** @addtogroup RpcP2P
* P2P 전용 RPC 인터페이스를 선언/정의한다
* @{
*/

// = P2P 전용 RPC 인터페이스 선언 매크로

/// 파라미터가 없는 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_0(method) \
    DECLARE_SRPC_METHOD_0(method); \
    void method##_remote(const void* rpcHint = 0)

/// 파라미터가 1개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_1(method, T1, p1) \
    DECLARE_SRPC_METHOD_1(method, T1, p1); \
    void method##_remote(const T1& p1, const void* rpcHint = 0)

/// 파라미터가 2개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_2(method, T1, p1, T2, p2) \
    DECLARE_SRPC_METHOD_2(method, T1, p1, T2, p2); \
    void method##_remote(const T1& p1, const T2& p2, const void* rpcHint = 0)

/// 파라미터가 3개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_3(method, T1, p1, T2, p2, \
        T3, p3) \
    DECLARE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const void* rpcHint = 0)

/// 파라미터가 4개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) \
    DECLARE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const void* rpcHint = 0)

/// 파라미터가 5개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5) \
    DECLARE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const void* rpcHint = 0)

/// 파라미터가 6개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5, T6, p6) \
    DECLARE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const void* rpcHint = 0)

/// 파라미터가 7개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_P2P_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5, T6, p6, T7, p7) \
    DECLARE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const T7& p7, \
        const void* rpcHint = 0)

// = DEFINE_SRPC_P2P_METHOD_n

/// 파라미터가 없는 RPC Method를 정의한다.
#define DEFINE_SRPC_P2P_METHOD_0(RpcClass, method, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, packetType)

/// 파라미터가 1개인 RPC Method를 정의한다
#define DEFINE_SRPC_P2P_METHOD_1(RpcClass, method, T1, p1, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, packetType)

/// 파라미터가 2개인 RPC Method를 정의한다
#define DEFINE_SRPC_P2P_METHOD_2(RpcClass, method, T1, p1, T2, p2, \
        packetType) \
    DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, \
        packetType)

/// 파라미터가 3개인 RPC Method를 정의한다
#define DEFINE_SRPC_P2P_METHOD_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType) \
    DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType)

/// 파라미터가 4개인 RPC Method를 정의한다
#define DEFINE_SRPC_P2P_METHOD_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType)

/// 파라미터가 5개인 RPC Method를 정의한다
#define DEFINE_SRPC_P2P_METHOD_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType)

/// 파라미터가 6개인 RPC Method를 정의한다
#define DEFINE_SRPC_P2P_METHOD_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType)

/// 파라미터가 7개인 RPC Method를 정의한다
#define DEFINE_SRPC_P2P_METHOD_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
    T4, p4, T5, p5, T6, p6, T7, p7, packetType)

// = IMPLEMENT_SRPC_P2P_METHOD_n

/// 파라미터가 없는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_0(RpcClass, method, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_0(), _remote); \
    DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, packetType); \
    void RpcClass::method##_remote(const void* rpcHint)

/// 파라미터가 1개 있는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_1(RpcClass, method, \
        T1, p1, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_1(T1), _remote) \
    DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, packetType); \
    void RpcClass::method##_remote(const T1& p1, const void* rpcHint)

/// 파라미터가 2개 있는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_2(RpcClass, method, \
        T1, p1, T2, p2, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_2(T1, T2), _remote) \
    DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, \
        packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, \
        const void* rpcHint)

/// 파라미터가 3개 있는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_3(T1, T2, T3), \
        _remote) \
    DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const void* rpcHint)

/// 파라미터가 4개 있는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_4(T1, T2, T3, T4), \
        _remote) \
    DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType); \
 void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const void* rpcHint)

/// 파라미터가 5개 있는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_5(RpcClass, method, \
    T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_5(T1, T2, T3, T4, T5), _remote) \
    DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const void* rpcHint)

/// 파라미터가 6개 있는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_6(T1, T2, T3, T4, T5, T6), _remote) \
    DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const void* rpcHint)

/// 파라미터가 7개 있는 P2P 전용 RPC Method를 구현한다
#define IMPLEMENT_SRPC_P2P_METHOD_7(RpcClass, method, \
    T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7), _remote) \
    DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const T7& p7, \
        const void* rpcHint)

/** @} */ // addtogroup RpcP2P

#endif // SRPC_RPCP2P_H
