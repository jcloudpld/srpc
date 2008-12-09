#ifndef SRPC_RPCINTERFACE_H
#define SRPC_RPCINTERFACE_H

#include "detail/RpcInterfaceDetail.h"

/** @addtogroup RpcInterface
* RPC �������̽��� ����/�����Ѵ�
* @{
*/

// = srpc method declaration macros

// = DECLARE_SRPC_PURE_METHOD_n

/// �Ķ���Ͱ� ���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_0(rpcInterface, method) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 0) \
    DECLARE_SRPC_METHOD_0(method) = 0

/// �Ķ���Ͱ� 1���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_1(rpcInterface, method, T1, p1) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 1) \
    DECLARE_SRPC_METHOD_1(method, T1, p1) = 0

/// �Ķ���Ͱ� 2���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_2(rpcInterface, method, T1, p1, T2, p2) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 2) \
    DECLARE_SRPC_METHOD_2(method, T1, p1, T2, p2) = 0

/// �Ķ���Ͱ� 3���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_3(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 3) \
    DECLARE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) = 0

/// �Ķ���Ͱ� 4���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_4(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 4) \
    DECLARE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) = 0

/// �Ķ���Ͱ� 5���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_5(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 5) \
    DECLARE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) = 0

/// �Ķ���Ͱ� 6���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_6(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 6) \
    DECLARE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) = 0

/// �Ķ���Ͱ� 7���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_PURE_METHOD_7(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 7) \
    DECLARE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) = 0

// = DECLARE_SRPC_METHOD_n

/// �Ķ���Ͱ� ���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_0(method) \
    virtual void method(const void* rpcHint = 0)

/// �Ķ���Ͱ� 1���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_1(method, T1, p1) \
    virtual void method(const T1& p1, const void* rpcHint = 0)

/// �Ķ���Ͱ� 2���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_2(method, T1, p1, T2, p2) \
    virtual void method(const T1& p1, const T2& p2, const void* rpcHint = 0)

/// �Ķ���Ͱ� 3���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) \
    virtual void method(const T1& p1, const T2& p2, const T3& p3, \
        const void* rpcHint = 0)

/// �Ķ���Ͱ� 4���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) \
    virtual void method(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const void* rpcHint = 0)

/// �Ķ���Ͱ� 5���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    virtual void method(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const void* rpcHint = 0)

/// �Ķ���Ͱ� 6���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) \
    virtual void method(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const void* rpcHint = 0)

/// �Ķ���Ͱ� 7���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) \
    virtual void method(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const T7& p7, \
        const void* rpcHint = 0)

/** @} */ // addtogroup RpcInterface

#endif // SRPC_RPCINTERFACE_H
