#ifndef SRPC_RPCID_H
#define SRPC_RPCID_H

#include "../RpcTypes.h"
#include "../utility/Hash.h"

namespace srpc {

/**
 * @class RpcId
 *
 * RPC ID
  */
class RpcId : public RUInt32
{
public:
    RpcId() {
        set(0);
    }

    RpcId(const char* idStr) {
        set(hash(idStr, strlen(idStr)));
    }

    RpcId(const String& idStr) {
        set(hash(idStr.c_str(), idStr.size()));
    }
};

} // namespace srpc

#endif // SRPC_RPCID_H
