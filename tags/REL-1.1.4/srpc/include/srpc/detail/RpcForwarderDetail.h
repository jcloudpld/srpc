#ifndef SRPC_RPCFORWARDERDETAIL_H
#define SRPC_RPCFORWARDERDETAIL_H

#include "RpcPrototype.h"
#include "RpcCommand.h"
#include "ForwardingFunctors.h"
#include "../RpcPacketType.h"

/** @addtogroup RpcForwarding
* RPC 송신
* @{
*/

// = Rpc request Macros

/// @internal
#define SRPC_RPC_COMMAND_CLASS(rpcClass, method) \
    RpcCommand_##rpcClass##_##method

// = DEFINE_SRPC_METHOD_DETAIL_n

/// 파라미터가 없는 RPC Method를 정의한다.
#define DEFINE_SRPC_METHOD_DETAIL_0(rpcClass, method, packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT<SRPC_TYPELIST_0() > FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)() : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_0(rpcClass, method) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command; \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 1개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_1(rpcClass, method, P1, p1, packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT<SRPC_TYPELIST_1(P1) > \
            FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)(const P1& p1) : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_), \
            marshalFunctor_(p1) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_1(rpcClass, method, P1, p1) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command(p1); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 2개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_2(rpcClass, method, P1, p1, P2, p2, \
        packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT<SRPC_TYPELIST_2(P1, P2) > \
            FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)(const P1& p1, \
            const P2& p2) : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_), \
            marshalFunctor_(p1, p2) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_2(rpcClass, method, P1, p1, P2, p2) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command(p1, p2); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 3개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_3(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT<SRPC_TYPELIST_3(P1, P2, P3) > \
            FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3) : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_), \
            marshalFunctor_(p1, p2, p3) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_3(rpcClass, method, P1, p1, P2, p2, \
        P3, p3) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command(p1, p2, p3); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 4개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_4(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT<SRPC_TYPELIST_4(P1, P2, P3, P4) > \
            FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4) : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_), \
            marshalFunctor_(p1, p2, p3, p4) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_4(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command(p1, p2, p3, p4); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 5개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_5(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT< \
            SRPC_TYPELIST_5(P1, P2, P3, P4, P5) > FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4, const P5& p5) : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_), \
            marshalFunctor_(p1, p2, p3, p4, p5) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_5(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command(p1, p2, p3, p4, p5); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 6개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_6(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT< \
            SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6) > FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4, const P5& p5, const P6& p6) : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_), \
            marshalFunctor_(p1, p2, p3, p4, p5, p6) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_6(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command( \
            p1, p2, p3, p4, p5,  p6); \
        forward(command, packetType, rpcHint); \
    }

/// 파라미터가 7개인 RPC Method를 정의한다
#define DEFINE_SRPC_METHOD_DETAIL_7(rpcClass, method, P1, p1, P2, p2, P3, p3, \
        P4, p4, P5, p5, P6, p6, P7, p7, packetType) \
    class SRPC_RPC_COMMAND_CLASS(rpcClass, method) : public srpc::RpcCommand \
    { \
    public: \
        typedef srpc::ForwardingFunctorT< \
            SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7) > FunctorType; \
    public: \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method)(const P1& p1, const P2& p2, \
            const P3& p3, const P4& p4, const P5& p5, const P6& p6, \
            const P7& p7) : \
            srpc::RpcCommand(rpcClass::SRPC_GET_RPCID(method)(), \
                marshalFunctor_), \
            marshalFunctor_(p1, p2, p3, p4, p5, p6, p7) {} \
    private: \
        FunctorType marshalFunctor_; \
    }; \
    SRPC_METHOD_PROTOTYPE_WITH_HINT_7(rpcClass, method, P1, p1, P2, p2, \
        P3, p3, P4, p4, P5, p5, P6, p6, P7, p7) \
    { \
        SRPC_RPC_COMMAND_CLASS(rpcClass, method) command( \
            p1, p2, p3, p4, p5, p6, p7); \
        forward(command, packetType, rpcHint); \
    }

/** @} */ // addtogroup RpcForwarding

#endif // SRPC_RPCFORWARDERDETAIL_H
