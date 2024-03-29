#ifndef SRPC_RPCINTERFACE_H
#define SRPC_RPCINTERFACE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "detail/RpcInterfaceDetail.h"
#include "detail/TypeList.h"

/** @addtogroup RpcInterface
* RPC 인터페이스를 선언/정의한다
* @{
*/

// = srpc method declaration macros

// = DECLARE_SRPC_METHOD_n

/// 파라미터가 없는 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_0(rpcInterface, method) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 0) \
    OVERRIDE_SRPC_METHOD_0(method) = 0

/// 파라미터가 1개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_1(rpcInterface, method, T1, p1) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 1) \
    OVERRIDE_SRPC_METHOD_1(method, T1, p1) = 0

/// 파라미터가 2개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_2(rpcInterface, method, T1, p1, T2, p2) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 2) \
    OVERRIDE_SRPC_METHOD_2(method, T1, p1, T2, p2) = 0

/// 파라미터가 3개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_3(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 3) \
    OVERRIDE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) = 0

/// 파라미터가 4개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_4(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 4) \
    OVERRIDE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) = 0

/// 파라미터가 5개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_5(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 5) \
    OVERRIDE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) = 0

/// 파라미터가 6개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_6(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 6) \
    OVERRIDE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) = 0

/// 파라미터가 7개인 RPC 메쏘드를 선언한다.
#define DECLARE_SRPC_METHOD_7(rpcInterface, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, 7) \
    OVERRIDE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) = 0

// = OVERRIDE_SRPC_METHOD_n

/// 파라미터가 없는 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_0(method) \
    virtual void method(const void* rpcHint = 0)

/// 파라미터가 1개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_1(method, T1, p1) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, const void* rpcHint = 0)

/// 파라미터가 2개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_2(method, T1, p1, T2, p2) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        const void* rpcHint = 0)

/// 파라미터가 3개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, const void* rpcHint = 0)

/// 파라미터가 4개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, const void* rpcHint = 0)

/// 파라미터가 5개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, SRPC_PARAM_TYPE(T5) p5, \
        const void* rpcHint = 0)

/// 파라미터가 6개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, SRPC_PARAM_TYPE(T5) p5, \
        SRPC_PARAM_TYPE(T6) p6, const void* rpcHint = 0)

/// 파라미터가 7개인 RPC 메쏘드를 선언한다.
#define OVERRIDE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7) \
    virtual void method(SRPC_PARAM_TYPE(T1) p1, SRPC_PARAM_TYPE(T2) p2, \
        SRPC_PARAM_TYPE(T3) p3, SRPC_PARAM_TYPE(T4) p4, SRPC_PARAM_TYPE(T5) p5, \
        SRPC_PARAM_TYPE(T6) p6, SRPC_PARAM_TYPE(T7) p7, const void* rpcHint = 0)

/** @} */ // addtogroup RpcInterface

#endif // SRPC_RPCINTERFACE_H
