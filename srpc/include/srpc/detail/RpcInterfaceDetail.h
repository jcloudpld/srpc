#ifndef SRPC_RPCINTERFACEDETAIL_H
#define SRPC_RPCINTERFACEDETAIL_H

#include "RpcId.h"

/** @addtogroup RpcInterface
* RPC 인터페이스를 선언/정의한다
* @{
*/

/// @internal
#define SRPC_STRINGIZE(text) # text

/// @internal
#define DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, paramCount) \
    public: \
        static const srpc::RpcId& getRpcId_##method() { \
            static srpc::RpcId rpcId( \
                SRPC_STRINGIZE(rpcInterface##_##method##_##paramCount)); \
            return rpcId; \
        }

/// @internal
#define SRPC_GET_RPCID(method) \
    getRpcId_ ## method

/** @} */ // addtogroup RpcInterface

#endif // SRPC_RPCINTERFACEDETAIL_H
