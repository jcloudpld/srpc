#ifndef NSRPC_RELAYSERVICEIMPL_H
#define NSRPC_RELAYSERVICEIMPL_H

#include <nsrpc/p2p/rpc_interface/RelayService.h>

namespace srpc
{
class RpcNetwork;
} // namespace srpc

namespace nsrpc
{

namespace detail
{

class RelayServiceHandler;

/** @addtogroup p2p
* @{
*/

/**
 * @class RpcRelayServiceImpl
 * P2P RelayService Impl
 */
class RpcRelayServiceImpl : public RpcRelayService,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(RpcRelayServiceImpl);
public:
    RpcRelayServiceImpl(RelayServiceHandler& serviceHandler,
        srpc::RpcNetwork& rpcNetwork);
public:
    OVERRIDE_SRPC_METHOD_6(rpcRelay,
        RPeerIdPair, peerIdPair, RAddress, peerAddress,
        RMessageBuffer, messageBlock, srpc::RRpcPacketType, packetType,
        SequenceNumber, sequenceNumber, PeerTime, sentTime);

    OVERRIDE_SRPC_P2P_METHOD_5(rpcRelayed,
        PeerId, peerId, RMessageBuffer, messageBlock,
        srpc::RRpcPacketType, packetType, SequenceNumber, sequenceNumber,
        PeerTime, sentTime);
private:
    RelayServiceHandler& serviceHandler_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_RELAYSERVICEIMPL_H
