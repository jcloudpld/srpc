#ifndef SRPC_RPCID_H
#define SRPC_RPCID_H

#include "../RpcTypes.h"
#include "../utility/Hash.h"

namespace srpc {

/// RPC ID
typedef UInt32 RpcId;

/**
 * @class RRpcId
 */
class RRpcId : public RpcUIntType<RpcId>
{
public:
    explicit RRpcId(RpcId rpcId = 0) {
        set(rpcId);
    }

    explicit RRpcId(const char* idStr) {
        set(hash(idStr, strlen(idStr)));
    }

    RRpcId(const String& idStr) {
        set(hash(idStr.c_str(), idStr.size()));
    }
};

} // namespace srpc

#endif // SRPC_RPCID_H
