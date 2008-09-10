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
    RPeerId peerId_;
    RAddresses addresses_;

    RPeerInfo() :
        peerId_(invalidPeerId) {}

    RPeerInfo(PeerId peerId, const RAddresses& addresses) :
        peerId_(peerId),
        addresses_(addresses) {}

    void write(srpc::OStream& ostream) {
        peerId_.write(ostream);
        addresses_.write(ostream);
    }

    void read(srpc::IStream& istream) {
        peerId_.read(istream);
        addresses_.read(istream);
    }
};


/// RPeerInfo list
typedef srpc::RVector<RPeerInfo> RPeerInfos;


/// RpcType for SequenceNumber
typedef srpc::RpcUIntType<SequenceNumber> RSequenceNumber;


/**
 * @class RpcSystemService
 * P2P �ý��� RPC �޼���
 */
class RpcSystemService
{
public:
    virtual ~RpcSystemService() {}

    /// ������ ��û�Ѵ�
    DECLARE_SRPC_PURE_METHOD_3(RpcSystemService, rpcConnect,
        RAddresses, peerAddresses, srpc::RShortString, sessionPassword,
        srpc::RUInt32, sessionKey);

    /// ���� ��û�� ���� ������ �Ѵ�
    DECLARE_SRPC_PURE_METHOD_3(RpcSystemService, rpcConnected,
        RAddresses, peerAddresses, srpc::RBool, isHost,
        RP2pProperty, p2pProperty);

    /// ȣ��Ʈ���� ���� ������ ��û�Ѵ�
    DECLARE_SRPC_PURE_METHOD_1(RpcSystemService, rpcRequestConnectReversal,
        RAddresses, peerAddresses);

    /// ȣ��Ʈ�� �Ǿ�� ���� ������ �õ��Ѵ�
    DECLARE_SRPC_PURE_METHOD_1(RpcSystemService, rpcConnectReversal,
        RAddresses, peerAddresses);

    /// ���ο� Peer�� �����Ͽ���.
    DECLARE_SRPC_PURE_METHOD_1(RpcSystemService, rpcNewPeerConnected,
        RPeerInfo, peerInfo);

    /// ���� ���ǿ��� ���� ������ �˸���.
    DECLARE_SRPC_PURE_METHOD_0(RpcSystemService, rpcDisconnect);

    /// �� �޼����� ������.
    DECLARE_SRPC_PURE_METHOD_0(RpcSystemService, rpcPing);

    /// �ŷ� ���� �޼����� ���� ����(ack)�� ������.
    DECLARE_SRPC_PURE_METHOD_2(RpcSystemService, rpcAcknowledgement,
        RSequenceNumber, sequenceNumber, RRelativeTime, sentTime);

    /// ȣ��Ʈ ���̱׷��̼��� �Ͼ��.
    DECLARE_SRPC_PURE_METHOD_0(RpcSystemService, rpcHostMigrated);

    /// �׷��� �����Ǿ����� �˸���
    DECLARE_SRPC_PURE_METHOD_1(RpcSystemService, rpcGroupCreated,
        RGroupInfo, groupInfo);

    /// �׷쿡 ���������� �˸���
    DECLARE_SRPC_PURE_METHOD_2(RpcSystemService, rpcGroupJoined,
        RGroupId, groupId, RPeerId, peerId);

    /// �׷쿡�� ���������� �˸���
    DECLARE_SRPC_PURE_METHOD_2(RpcSystemService, rpcGroupLeft,
        RGroupId, groupId, RPeerId, peerId);
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_SYSTEMSERVICE_H
