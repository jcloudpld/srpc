#ifndef SRPC_RPCFORWARDERDETAIL_H
#define SRPC_RPCFORWARDERDETAIL_H

#include "RpcCommand.h"
#include "ForwardingFunctors.h"
#include "../RpcPrototype.h"
#include "../RpcPacketType.h"

/** @addtogroup RpcForwarding
* RPC 송신
* @{
*/

// = Rpc request Macros

/// @internal
#define SRPC_RPC_COMMAND_CLASS(RpcClass, method) \
    RpcCommand_##RpcClass##_##method

// = DEFINE_SRPC_METHOD_DETAIL_n

/// 파라미터가 없는 RPC Method를 정의한다.
#define DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)() : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_0() > marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_0(RpcClass, method) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command; \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 1개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        P1, p1, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)(const P1& p1) : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()), \
            marshalFunctor_(p1) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_1(P1) > marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_1(RpcClass, method, P1, p1) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command(p1); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 2개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        P1, p1, P2, p2, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)(const P1& p1, \
            const P2& p2) : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()), \
            marshalFunctor_(p1, p2) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_2(P1, P2) > marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_2(RpcClass, method, P1, p1, P2, p2) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command(p1, p2); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 3개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3) : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()), \
            marshalFunctor_(p1, p2, p3) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_3(P1, P2, P3) > \
            marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_3(RpcClass, method, P1, p1, P2, p2, \
        P3, p3) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command(p1, p2, p3); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 4개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4) : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()), \
            marshalFunctor_(p1, p2, p3, p4) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_4(P1, P2, P3, P4) > \
            marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_4(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command(p1, p2, p3, p4); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 5개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4, const P5& p5) : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()), \
            marshalFunctor_(p1, p2, p3, p4, p5) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_5(P1, P2, P3, P4, P5) > \
            marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_5(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command(p1, p2, p3, p4, p5); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 6개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4, const P5& p5, const P6& p6) : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()), \
            marshalFunctor_(p1, p2, p3, p4, p5, p6) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT<SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6) > \
            marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_6(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command( \
            p1, p2, p3, p4, p5,  p6); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 7개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, packetType) \
    class SRPC_RPC_COMMAND_CLASS(RpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4, const P5& p5, const P6& p6, \
            const P7& p7) : \
            srpc::RpcCommand(RpcClass::SRPC_GET_RPCID(method)()), \
            marshalFunctor_(p1, p2, p3, p4, p5, p6, p7) {} \
    private: \
        virtual srpc::ForwardingFunctor& getMarshaler() { \
            return marshalFunctor_; \
        } \
    private: \
        srpc::ForwardingFunctorT< \
            SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7) > marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_7(RpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7) \
    { \
        SRPC_RPC_COMMAND_CLASS(RpcClass, method) command( \
            p1, p2, p3, p4, p5, p6, p7); \
        forward(command, packetType, rpcHint); \
    }

/** @} */ // addtogroup RpcForwarding

#endif // SRPC_RPCFORWARDERDETAIL_H
