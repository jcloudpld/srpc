#ifndef NSRPC_STUNSERVICE_H
#define NSRPC_STUNSERVICE_H

#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <srpc/RpcP2p.h>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class RpcStunService
 * P2P STUN RPC �޼���
 */
class RpcStunService
{
public:
    virtual ~RpcStunService() {}

    // = request

    /// address resolving�� ��û�Ѵ�
    DECLARE_SRPC_PURE_METHOD_0(RpcStunService, rpcResolve);

    // = response

    /// address resolving�� ���� ����
    DECLARE_SRPC_PURE_METHOD_2(RpcStunService, rpcResolved,
        srpc::RShortString, ipAddress, srpc::RUInt16, port);
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_STUNSERVICE_H
