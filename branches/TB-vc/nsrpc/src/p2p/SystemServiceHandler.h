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

    /// 세션 인증
    virtual bool authenticate(PeerId peerId,
        const srpc::String& sessionPassword, srpc::UInt32 sessionKey) = 0;

    /**
     * 새로운 피어가 접속하였다.
     * @return 새로운 피어일 경우 true.
     */
    virtual bool peerConnected(PeerId peerId,
        const ACE_INET_Addr& targetAddress,
        const RAddresses& peerAddresses) = 0;

    /**
     * 피어가 접속을 해제하였다.
     */
    virtual void peerDisconnected(PeerId peerId) = 0;

    /// 해당 피어에 접속을 시도한다.
    virtual void connectToNewPeer(PeerId peerId,
        const Addresses& peerAddresses) = 0;

    /// peerId가 새로운 호스트가 되었다
    virtual void hostMigrated(PeerId peerId) = 0;

    virtual void setGroups(const RGroupMap& groups) = 0;

    /// P2P 그룹이 생성되었다
    virtual void groupCreated(const RGroupInfo& groupInfo) = 0;

    /// P2P 그룹에 참여하였다
    virtual void groupJoined(GroupId groupId, PeerId peerId) = 0;

    /// P2P 그룹에서 퇴장하였다
    virtual void groupLeft(GroupId groupId, PeerId peerId) = 0;

    /// P2P 속성을 설정한다
    virtual void setP2pProperty(const RP2pProperty& p2pProperty) = 0;

    /// 역 접속을 해야 하는가?
    virtual bool shouldConnectReversal(const ACE_INET_Addr& address) const = 0;

    /// 호스트에 접속했는가?
    virtual bool isHostConnected() const = 0;

    virtual const RGroupMap& getCurrentGroups() const = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_SYSTEMSERVICEHANDLER_H
