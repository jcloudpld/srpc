#ifndef NSRPC_RELAYSERVICE_H
#define NSRPC_RELAYSERVICE_H

#include "../detail/P2pRpcTypes.h"
#include "../detail/P2pProtocol.h"
#include "../PeerId.h"
#include <srpc/RpcP2p.h>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class RpcRelayService
 * P2P Relay RPC �޼���
 */
class RpcRelayService
{
public:
    virtual ~RpcRelayService() {}

    // = request

    /// relay�� ��û�Ѵ�
    DECLARE_SRPC_METHOD_6(RpcRelayService, rpcRelay,
        RPeerIdPair, peerIdPair, RAddress, peerAddress,
        RMessageBuffer, messageBlock, srpc::RRpcPacketType, packetType,
        SequenceNumber, sequenceNumber, PeerTime, sentTime);

    // = event

    /// relay �޼����� �����ߴ�
    DECLARE_SRPC_METHOD_5(RpcRelayService, rpcRelayed,
        PeerId, peerId, RMessageBuffer, messageBlock,
        srpc::RRpcPacketType, packetType, SequenceNumber, sequenceNumber,
        PeerTime, sentTime);
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_RELAYSERVICE_H
