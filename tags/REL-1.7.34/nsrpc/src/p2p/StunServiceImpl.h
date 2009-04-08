#ifndef NSRPC_STUNSERVICEIMPL_H
#define NSRPC_STUNSERVICEIMPL_H

#include <nsrpc/p2p/rpc_interface/StunService.h>

namespace srpc
{
class RpcNetwork;
} // namespace srpc

namespace nsrpc
{

namespace detail
{

class StunServiceHandler;

/** @addtogroup p2p
* @{
*/

/**
 * @class RpcStunServiceImpl
 * P2P StunService Impl
 */
class RpcStunServiceImpl : public RpcStunService,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(RpcStunServiceImpl);
public:
    RpcStunServiceImpl(StunServiceHandler& serviceHandler,
        srpc::RpcNetwork& rpcNetwork);

    /**
     * ������ ������ �����Ǿ���
     * - NAT�� �ʱ� ������ ���� ���� �õ����� ���� ��� ��Ʈ ������ ���� �ʴ�
     *   ������ �����ϱ� ����
     */
    void relayServerSetUp();

    void resolve();
public:
    DECLARE_SRPC_METHOD_0(rpcResolve);
    DECLARE_SRPC_P2P_METHOD_2(rpcResolved,
        srpc::RShortString, ipAddress, srpc::RUInt16, port);
private:
    StunServiceHandler& serviceHandler_;

    PeerTime lastResolvingTime_; ///< �ֱ� resolving ��û �ð�
    PeerTime lastResolvedTime_; ///< �ֱ� resolving�� �ð�
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_STUNSERVICEIMPL_H
