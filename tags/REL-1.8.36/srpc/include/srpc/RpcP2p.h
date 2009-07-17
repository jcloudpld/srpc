#ifndef SRPC_RPCP2P_H
#define SRPC_RPCP2P_H

#include "RpcReceiver.h"
#include "RpcForwarder.h"
#include "RpcInterface.h"

/** @addtogroup RpcP2P
* P2P ���� RPC �������̽��� ����/�����Ѵ�
* @{
*/

// = P2P ���� RPC �������̽� ���� ��ũ��

/// �Ķ���Ͱ� ���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_0(method) \
    OVERRIDE_SRPC_METHOD_0(method); \
    void method##_remote(const void* rpcHint = 0)

/// �Ķ���Ͱ� 1���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_1(method, T1, p1) \
    OVERRIDE_SRPC_METHOD_1(method, T1, p1); \
    void method##_remote(const T1& p1, const void* rpcHint = 0)

/// �Ķ���Ͱ� 2���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_2(method, T1, p1, T2, p2) \
    OVERRIDE_SRPC_METHOD_2(method, T1, p1, T2, p2); \
    void method##_remote(const T1& p1, const T2& p2, const void* rpcHint = 0)

/// �Ķ���Ͱ� 3���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_3(method, T1, p1, T2, p2, \
        T3, p3) \
    OVERRIDE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const void* rpcHint = 0)

/// �Ķ���Ͱ� 4���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) \
    OVERRIDE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const void* rpcHint = 0)

/// �Ķ���Ͱ� 5���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5) \
    OVERRIDE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const void* rpcHint = 0)

/// �Ķ���Ͱ� 6���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5, T6, p6) \
    OVERRIDE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const void* rpcHint = 0)

/// �Ķ���Ͱ� 7���� P2P ���� RPC �޽�带 �����Ѵ�.
#define OVERRIDE_SRPC_P2P_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5, T6, p6, T7, p7) \
    OVERRIDE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const T7& p7, \
        const void* rpcHint = 0)

// = FORWARD_SRPC_P2P_METHOD_n

/// �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�.
#define FORWARD_SRPC_P2P_METHOD_0(RpcClass, method, packetType) \
    FORWARD_SRPC_METHOD_DETAIL_0(RpcClass, method, packetType)

/// �Ķ���Ͱ� 1���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_P2P_METHOD_1(RpcClass, method, T1, p1, packetType) \
    FORWARD_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, packetType)

/// �Ķ���Ͱ� 2���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_P2P_METHOD_2(RpcClass, method, T1, p1, T2, p2, \
        packetType) \
    FORWARD_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, \
        packetType)

/// �Ķ���Ͱ� 3���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_P2P_METHOD_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType) \
    FORWARD_SRPC_METHOD_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType)

/// �Ķ���Ͱ� 4���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_P2P_METHOD_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType) \
    FORWARD_SRPC_METHOD_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType)

/// �Ķ���Ͱ� 5���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_P2P_METHOD_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType) \
    FORWARD_SRPC_METHOD_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType)

/// �Ķ���Ͱ� 6���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_P2P_METHOD_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType) \
    FORWARD_SRPC_METHOD_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType)

/// �Ķ���Ͱ� 7���� RPC Method�� �����Ѵ�
#define FORWARD_SRPC_P2P_METHOD_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, packetType) \
    FORWARD_SRPC_METHOD_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
    T4, p4, T5, p5, T6, p6, T7, p7, packetType)

// = EXCHANGE_SRPC_P2P_METHOD_n

/// �Ķ���Ͱ� ���� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_0(RpcClass, method, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_0(), _remote); \
    FORWARD_SRPC_METHOD_DETAIL_0(RpcClass, method, packetType); \
    void RpcClass::method##_remote(const void* rpcHint)

/// �Ķ���Ͱ� 1�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_1(RpcClass, method, \
        T1, p1, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_1(T1), _remote) \
    FORWARD_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, packetType); \
    void RpcClass::method##_remote(const T1& p1, const void* rpcHint)

/// �Ķ���Ͱ� 2�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_2(RpcClass, method, \
        T1, p1, T2, p2, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_2(T1, T2), _remote) \
    FORWARD_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, \
        packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, \
        const void* rpcHint)

/// �Ķ���Ͱ� 3�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_3(T1, T2, T3), \
        _remote) \
    FORWARD_SRPC_METHOD_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const void* rpcHint)

/// �Ķ���Ͱ� 4�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_4(T1, T2, T3, T4), \
        _remote) \
    FORWARD_SRPC_METHOD_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType); \
 void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const void* rpcHint)

/// �Ķ���Ͱ� 5�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_5(RpcClass, method, \
    T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_5(T1, T2, T3, T4, T5), _remote) \
    FORWARD_SRPC_METHOD_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const void* rpcHint)

/// �Ķ���Ͱ� 6�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_6(T1, T2, T3, T4, T5, T6), _remote) \
    FORWARD_SRPC_METHOD_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const void* rpcHint)

/// �Ķ���Ͱ� 7�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define EXCHANGE_SRPC_P2P_METHOD_7(RpcClass, method, \
    T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, packetType) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7), _remote) \
    FORWARD_SRPC_METHOD_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, packetType); \
    void RpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const T7& p7, \
        const void* rpcHint)

/** @} */ // addtogroup RpcP2P

#endif // SRPC_RPCP2P_H
