#ifndef SRPC_RPCID_H
#define SRPC_RPCID_H

#include "../RpcTypes.h"
#include "../utility/Hash.h"
#include <cstring>

namespace srpc {

/// RPC ID
typedef UInt32 RpcId;

/**
 * @class RRpcId
 * RPC type for RPC ID
 */
class RRpcId : public RpcUIntType<RpcId>
{
public:
    explicit RRpcId(RpcId rpcId = 0) :
        methodName_("unknown") {
        set(rpcId);
    }

    RRpcId(const char* id, const char* methodName) :
        methodName_(methodName) {
        set(hash(id, strlen(id)));
    }

    RRpcId(const String& id) {
        set(hash(id.c_str(), id.size()));
    }

    const char* getMethodName() const {
        return methodName_;
    }
private:
    const char* methodName_;
};

} // namespace srpc

#endif // SRPC_RPCID_H