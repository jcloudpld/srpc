#ifndef SRPC_RPCINTERFACEDETAIL_H
#define SRPC_RPCINTERFACEDETAIL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "RpcId.h"

/** @addtogroup RpcInterface
* RPC �������̽��� ����/�����Ѵ�
* @{
*/

/// @internal
#define SRPC_STRINGIZE(text) # text

/// @internal
#define SRPC_GET_RPCID(method) \
    getRpcId_ ## method

/// @internal
#define DECLARE_SRPC_RPCID_GENERATOR(rpcInterface, method, paramCount) \
    public: \
        static const srpc::RRpcId& SRPC_GET_RPCID(method)() { \
            static srpc::RRpcId rpcId( \
                SRPC_STRINGIZE(rpcInterface##_##method##_##paramCount), \
                SRPC_STRINGIZE(method)); \
            return rpcId; \
        }

/** @} */ // addtogroup RpcInterface

#endif // SRPC_RPCINTERFACEDETAIL_H
