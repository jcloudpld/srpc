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
     * 릴레이 서버가 설정되었다
     * - NAT에 초기 접속을 여러 차례 시도하지 않을 경우 포트 매핑이 되지 않는
     *   문제를 보완하기 위해
     */
    void relayServerSetUp();

    void resolve();
public:
    DECLARE_SRPC_METHOD_0(rpcResolve);
    DECLARE_SRPC_P2P_METHOD_2(rpcResolved,
        srpc::RShortString, ipAddress, srpc::RUInt16, port);
private:
    StunServiceHandler& serviceHandler_;

    PeerTime lastResolvingTime_; ///< 최근 resolving 요청 시간
    PeerTime lastResolvedTime_; ///< 최근 resolving된 시간
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_STUNSERVICEIMPL_H
