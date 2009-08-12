#ifndef NSRPC_SYSTEMSERVICE_H
#define NSRPC_SYSTEMSERVICE_H

#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <nsrpc/p2p/detail/P2pRpcTypes.h>
#include <nsrpc/p2p/Group.h>
#include <srpc/RpcP2p.h>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @struct RPeerInfo
 * Peer 정보
 */
struct RPeerInfo
{
    PeerId peerId_;
    RAddresses addresses_;
    RP2pOptions p2pOptions_;

    RPeerInfo() :
        peerId_(invalidPeerId) {}

    RPeerInfo(PeerId peerId, const RAddresses& addresses,
        P2pOptions p2pOptions) :
        peerId_(peerId),
        addresses_(addresses),
        p2pOptions_(p2pOptions) {}

    void write(srpc::OStream& ostream) {
        ostream.write(peerId_);
        addresses_.write(ostream);
        p2pOptions_.write(ostream);
    }

    void read(srpc::IStream& istream) {
        istream.read(peerId_);
        addresses_.read(istream);
        p2pOptions_.read(istream);
    }
};


/// RPeerInfo list
typedef srpc::RVector<RPeerInfo> RPeerInfos;


/**
 * @class RpcSystemService
 * P2P 시스템 RPC 메세지
 */
class RpcSystemService
{
public:
    virtual ~RpcSystemService() {}

    /// 연결을 요청한다
    DECLARE_SRPC_METHOD_4(RpcSystemService, rpcConnect,
        RAddresses, peerAddresses, RP2pOptions, p2pOptions,
        srpc::RShortString, sessionPassword,
        srpc::RUInt32, sessionKey);

    /// 연결 요청에 대한 응답을 한다
    DECLARE_SRPC_METHOD_5(RpcSystemService, rpcConnected,
        RAddresses, peerAddresses, srpc::RBool, isHost,
        RP2pProperty, p2pProperty, RP2pOptions, P2pOptions, RGroupMap, groups);

    /// 호스트에게 직접 연결을 요청한다
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcRequestConnectReversal,
        RAddresses, peerAddresses);

    /// 호스트가 피어에게 직접 연결을 시도한다
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcConnectReversal,
        RAddresses, PeerAddresses, RP2pOptions, p2pOptions);

    /// 새로운 Peer가 접속하였다.
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcNewPeerConnected,
        RPeerInfo, peerInfo);

    /// 현재 세션에서 접속 해제를 알린다.
    DECLARE_SRPC_METHOD_0(RpcSystemService, rpcDisconnect);

    /// 핑 메세지를 보낸다.
    DECLARE_SRPC_METHOD_0(RpcSystemService, rpcPing);

    /// 신뢰 보장 메세지에 대한 응답(ack)를 보낸다.
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcAcknowledgement,
        SequenceNumber, sequenceNumber, PeerTime, sentTime);

    /// 호스트 마이그레이션이 일어났다.
    DECLARE_SRPC_METHOD_0(RpcSystemService, rpcHostMigrated);

    /// 그룹이 생성되었음을 알린다
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcGroupCreated,
        RGroupInfo, groupInfo);

    /// 그룹이 파괴되었음을 알린다
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcGroupDestroyed,
        RGroupId, groupId);

    /// 그룹에 참여했음을 알린다
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcGroupJoined,
        RGroupId, groupId, PeerId, peerId);

    /// 그룹에서 퇴장했음을 알린다
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcGroupLeft,
        RGroupId, groupId, PeerId, peerId);
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_SYSTEMSERVICE_H
