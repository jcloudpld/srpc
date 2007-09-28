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

/// �Ķ���Ͱ� ���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_0(method) \
    DECLARE_SRPC_METHOD_0(method); \
    void method##_remote(const void* rpcHint = 0)

/// �Ķ���Ͱ� 1���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_1(method, T1, p1) \
    DECLARE_SRPC_METHOD_1(method, T1, p1); \
    void method##_remote(const T1& p1, const void* rpcHint = 0)

/// �Ķ���Ͱ� 2���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_2(method, T1, p1, T2, p2) \
    DECLARE_SRPC_METHOD_2(method, T1, p1, T2, p2); \
    void method##_remote(const T1& p1, const T2& p2, const void* rpcHint = 0)

/// �Ķ���Ͱ� 3���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_3(method, T1, p1, T2, p2, \
        T3, p3) \
    DECLARE_SRPC_METHOD_3(method, T1, p1, T2, p2, T3, p3); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const void* rpcHint = 0)

/// �Ķ���Ͱ� 4���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4) \
    DECLARE_SRPC_METHOD_4(method, T1, p1, T2, p2, T3, p3, T4, p4); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const void* rpcHint = 0)

/// �Ķ���Ͱ� 5���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5) \
    DECLARE_SRPC_METHOD_5(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const void* rpcHint = 0)

/// �Ķ���Ͱ� 6���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5, T6, p6) \
    DECLARE_SRPC_METHOD_6(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const void* rpcHint = 0)

/// �Ķ���Ͱ� 7���� RPC �޽�带 �����Ѵ�.
#define DECLARE_SRPC_P2P_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, \
        T5, p5, T6, p6, T7, p7) \
    DECLARE_SRPC_METHOD_7(method, T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, \
        T6, p6, T7, p7); \
    void method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const T7& p7, \
        const void* rpcHint = 0)

// = DEFINE_SRPC_P2P_METHOD_n

/// �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�.
#define DEFINE_SRPC_P2P_METHOD_0(rpcClass, method, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_0(rpcClass, method, packetType)

/// �Ķ���Ͱ� 1���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_P2P_METHOD_1(rpcClass, method, T1, p1, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_1(rpcClass, method, T1, p1, packetType)

/// �Ķ���Ͱ� 2���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_P2P_METHOD_2(rpcClass, method, T1, p1, T2, p2, \
        packetType) \
    DEFINE_SRPC_METHOD_DETAIL_2(rpcClass, method, T1, p1, T2, p2, \
        packetType)

/// �Ķ���Ͱ� 3���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_P2P_METHOD_3(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType) \
    DEFINE_SRPC_METHOD_DETAIL_3(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType)

/// �Ķ���Ͱ� 4���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_P2P_METHOD_4(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_4(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType)

/// �Ķ���Ͱ� 5���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_P2P_METHOD_5(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_5(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType)

/// �Ķ���Ͱ� 6���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_P2P_METHOD_6(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_6(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType)

/// �Ķ���Ͱ� 7���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_P2P_METHOD_7(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, packetType) \
    DEFINE_SRPC_METHOD_DETAIL_7(rpcClass, method, T1, p1, T2, p2, T3, p3, \
    T4, p4, T5, p5, T6, p6, T7, p7, packetType)

// = IMPLEMENT_SRPC_P2P_METHOD_n

/// �Ķ���Ͱ� ���� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_0(rpcClass, method, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, SRPC_TYPELIST_0() > \
            FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method); \
    DEFINE_SRPC_METHOD_DETAIL_0(rpcClass, method, packetType); \
    void rpcClass::method##_remote(const void* rpcHint)

/// �Ķ���Ͱ� 1�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_1(rpcClass, method, \
        T1, p1, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, SRPC_TYPELIST_1(T1) > \
            FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_1(rpcClass, method, T1, p1, packetType); \
    void rpcClass::method##_remote(const T1& p1, const void* rpcHint)

/// �Ķ���Ͱ� 2�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_2(rpcClass, method, \
        T1, p1, T2, p2, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_2(T1, T2) > FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_2(rpcClass, method, T1, p1, T2, p2, \
        packetType); \
    void rpcClass::method##_remote(const T1& p1, const T2& p2, \
        const void* rpcHint)

/// �Ķ���Ͱ� 3�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_3(T1, T2, T3) > FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_3(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        packetType); \
    void rpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const void* rpcHint)

/// �Ķ���Ͱ� 4�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_4(T1, T2, T3, T4) > FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_4(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, packetType); \
 void rpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const void* rpcHint)

/// �Ķ���Ͱ� 5�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_5(rpcClass, method, \
    T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_5(T1, T2, T3, T4, T5) > FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_5(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, packetType); \
    void rpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const void* rpcHint)

/// �Ķ���Ͱ� 6�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
    typedef srpc::ReceivingFunctorT<rpcClass, \
        SRPC_TYPELIST_6(T1, T2, T3, T4, T5, T6) > FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_6(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, packetType); \
    void rpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const void* rpcHint)

/// �Ķ���Ͱ� 7�� �ִ� P2P ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_P2P_METHOD_7(rpcClass, method, \
    T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, packetType) \
    class RpcEvent_##rpcClass##_##method : public srpc::RpcEvent \
    { \
    public: \
    typedef srpc::ReceivingFunctorT<rpcClass, \
        SRPC_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) > FunctorType; \
    public: \
        RpcEvent_##rpcClass##_##method() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method##_remote) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new RpcEvent_##rpcClass##_##method; \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_7(rpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, packetType); \
    void rpcClass::method##_remote(const T1& p1, const T2& p2, const T3& p3, \
        const T4& p4, const T5& p5, const T6& p6, const T7& p7, \
        const void* rpcHint)

/** @} */ // addtogroup RpcP2P

#endif // SRPC_RPCP2P_H
