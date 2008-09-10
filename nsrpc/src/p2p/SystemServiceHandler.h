#ifndef NSRPC_SYSTEMSERVICEHANDLER_H
#define NSRPC_SYSTEMSERVICEHANDLER_H

#include <nsrpc/p2p/Group.h>
#include <nsrpc/p2p/detail/P2pAddress.h>
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class SystemServiceHandler
 *
 * P2P system service handler
 */
class SystemServiceHandler : public boost::noncopyable
{
public:
    virtual ~SystemServiceHandler() {}

    /// ���� ����
    virtual bool authenticate(PeerId peerId,
        const srpc::String& sessionPassword, srpc::UInt32 sessionKey) = 0;

    /**
     * ���ο� �Ǿ �����Ͽ���.
     * @return ���ο� �Ǿ��� ��� true.
     */
    virtual bool peerConnected(PeerId peerId,
        const ACE_INET_Addr& targetAddress,
        const RAddresses& peerAddresses) = 0;

    /**
     * �Ǿ ������ �����Ͽ���.
     */
    virtual void peerDisconnected(PeerId peerId) = 0;

    /// �ش� �Ǿ ������ �õ��Ѵ�.
    virtual void connectToNewPeer(PeerId peerId,
        const Addresses& peerAddresses) = 0;

    /// peerId�� ���ο� ȣ��Ʈ�� �Ǿ���
    virtual void hostMigrated(PeerId peerId) = 0;

    virtual void setGroups(const RGroupMap& groups) = 0;

    /// P2P �׷��� �����Ǿ���
    virtual void groupCreated(const RGroupInfo& groupInfo) = 0;

    /// P2P �׷쿡 �����Ͽ���
    virtual void groupJoined(GroupId groupId, PeerId peerId) = 0;

    /// P2P �׷쿡�� �����Ͽ���
    virtual void groupLeft(GroupId groupId, PeerId peerId) = 0;

    /// P2P �Ӽ��� �����Ѵ�
    virtual void setP2pProperty(const RP2pProperty& p2pProperty) = 0;

    /// �� ������ �ؾ� �ϴ°�?
    virtual bool shouldConnectReversal(const ACE_INET_Addr& address) const = 0;

    /// ȣ��Ʈ�� �����ߴ°�?
    virtual bool isHostConnected() const = 0;

    virtual const RGroupMap& getCurrentGroups() const = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_SYSTEMSERVICEHANDLER_H
