#ifndef NSRPC_SYSTEMSERVICEIMPL_H
#define NSRPC_SYSTEMSERVICEIMPL_H

#include "SystemService.h"

namespace srpc
{
class RpcNetwork;
} // namespace srpc

namespace nsrpc
{

class P2pEventHandler;

namespace detail
{

class PeerManager;
class SystemServiceHandler;
struct P2pPeerHint;

/** @addtogroup p2p
* @{
*/

/**
 * @class RpcSystemServiceImpl
 * P2P SystemService Impl
 */
class RpcSystemServiceImpl : public RpcSystemService,
    private srpc::RpcReceiver, private srpc::RpcForwarder
{
    DECLARE_SRPC_EVENT_DISPATCHER(RpcSystemServiceImpl);
public:
    RpcSystemServiceImpl(PeerManager& peers, P2pEventHandler& eventHandler,
        SystemServiceHandler& serviceHandler, srpc::RpcNetwork& rpcNetwork);

    void setHost(const RP2pProperty& p2pProperty);
    void reset();
public:
    DECLARE_SRPC_P2P_METHOD_3(rpcConnect, RAddresses, peerAddresses,
        srpc::RShortString, sessionPassword, srpc::RUInt32, sessionKey);
    DECLARE_SRPC_P2P_METHOD_4(rpcConnected, RAddresses, peerAddresses,
        srpc::RBool, isHost, RP2pProperty, p2pProperty, RGroupMap, groups);
    DECLARE_SRPC_P2P_METHOD_1(rpcRequestConnectReversal,
        RAddresses, peerAddresses);
    DECLARE_SRPC_P2P_METHOD_1(rpcConnectReversal,
        RAddresses, peerAddresses);
    DECLARE_SRPC_P2P_METHOD_1(rpcNewPeerConnected, RPeerInfo, peerInfo);
    DECLARE_SRPC_P2P_METHOD_0(rpcDisconnect);
    DECLARE_SRPC_P2P_METHOD_0(rpcPing);
    DECLARE_SRPC_P2P_METHOD_2(rpcAcknowledgement,
        RSequenceNumber, sequenceNumber, RRelativeTime, sentTime);
    DECLARE_SRPC_P2P_METHOD_0(rpcHostMigrated);
    DECLARE_SRPC_P2P_METHOD_1(rpcGroupCreated,
        RGroupInfo, groupInfo);
    DECLARE_SRPC_P2P_METHOD_1(rpcGroupDestroyed,
        RGroupId, groupId);
    DECLARE_SRPC_P2P_METHOD_2(rpcGroupJoined,
        RGroupId, groupId, RPeerId, peerId);
    DECLARE_SRPC_P2P_METHOD_2(rpcGroupLeft,
        RGroupId, groupId, RPeerId, peerId);
private:
    void connectToNewPeer(const RPeerInfo& pi);
    void reconnectToPeer(PeerId peerId, const RAddresses& peerAddresses);
    void logRpc(const char* rpc, const P2pPeerHint& hint,
        SequenceNumber sequenceNumber = 0, PeerTime sentTime = 0);
private:
    PeerManager& peerManager_;
    P2pEventHandler& eventHandler_;
    SystemServiceHandler& serviceHandler_;

    bool isHost_;
    RP2pProperty p2pProperty_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_SYSTEMSERVICEIMPL_H
