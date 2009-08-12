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
 * Peer ����
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
 * P2P �ý��� RPC �޼���
 */
class RpcSystemService
{
public:
    virtual ~RpcSystemService() {}

    /// ������ ��û�Ѵ�
    DECLARE_SRPC_METHOD_4(RpcSystemService, rpcConnect,
        RAddresses, peerAddresses, RP2pOptions, p2pOptions,
        srpc::RShortString, sessionPassword,
        srpc::RUInt32, sessionKey);

    /// ���� ��û�� ���� ������ �Ѵ�
    DECLARE_SRPC_METHOD_5(RpcSystemService, rpcConnected,
        RAddresses, peerAddresses, srpc::RBool, isHost,
        RP2pProperty, p2pProperty, RP2pOptions, P2pOptions, RGroupMap, groups);

    /// ȣ��Ʈ���� ���� ������ ��û�Ѵ�
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcRequestConnectReversal,
        RAddresses, peerAddresses);

    /// ȣ��Ʈ�� �Ǿ�� ���� ������ �õ��Ѵ�
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcConnectReversal,
        RAddresses, PeerAddresses, RP2pOptions, p2pOptions);

    /// ���ο� Peer�� �����Ͽ���.
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcNewPeerConnected,
        RPeerInfo, peerInfo);

    /// ���� ���ǿ��� ���� ������ �˸���.
    DECLARE_SRPC_METHOD_0(RpcSystemService, rpcDisconnect);

    /// �� �޼����� ������.
    DECLARE_SRPC_METHOD_0(RpcSystemService, rpcPing);

    /// �ŷ� ���� �޼����� ���� ����(ack)�� ������.
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcAcknowledgement,
        SequenceNumber, sequenceNumber, PeerTime, sentTime);

    /// ȣ��Ʈ ���̱׷��̼��� �Ͼ��.
    DECLARE_SRPC_METHOD_0(RpcSystemService, rpcHostMigrated);

    /// �׷��� �����Ǿ����� �˸���
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcGroupCreated,
        RGroupInfo, groupInfo);

    /// �׷��� �ı��Ǿ����� �˸���
    DECLARE_SRPC_METHOD_1(RpcSystemService, rpcGroupDestroyed,
        RGroupId, groupId);

    /// �׷쿡 ���������� �˸���
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcGroupJoined,
        RGroupId, groupId, PeerId, peerId);

    /// �׷쿡�� ���������� �˸���
    DECLARE_SRPC_METHOD_2(RpcSystemService, rpcGroupLeft,
        RGroupId, groupId, PeerId, peerId);
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_SYSTEMSERVICE_H
