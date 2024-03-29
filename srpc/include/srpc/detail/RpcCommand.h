#ifndef SRPC_RPCCOMMAND_H
#define SRPC_RPCCOMMAND_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../srpc.h"
#include "../Types.h"
#include "../detail/RpcId.h"
#include <boost/utility.hpp>

namespace srpc {

class OStream;
class ForwardingFunctor;

/** @addtogroup RpcForwarding
* @{
*/

/**
 * @class RpcCommand
 *
 * rpc command(request)
 * - 요청하는 쪽의 부하를 줄이기 위해 필요한 정보를 wrapping
 */
class SRPC_API RpcCommand : public boost::noncopyable
{
public:
    RpcCommand(const RRpcId& rpcId, ForwardingFunctor& marshaler)  :
        rpcId_(rpcId),
        marshaler_(marshaler) {}

    virtual ~RpcCommand() {}

    /**
     * RPC command를 ostream으로 마샬링한다
     * @exception StreamingException 스트리밍에 실패할 경우
     */
    void marshal(OStream& ostream);

    const RRpcId& getRpcId() const {
        return rpcId_;
    }

private:
    const RRpcId& rpcId_;
    ForwardingFunctor& marshaler_;
};

/** @} */ // addtogroup RpcForwarding

} // namespace srpc

#endif // SRPC_RPCCOMMAND_H
