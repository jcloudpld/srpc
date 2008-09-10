#ifndef NSRPC_P2PEVENTHANDLER_H
#define NSRPC_P2PEVENTHANDLER_H

#include "Group.h"
#include <boost/noncopyable.hpp>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @class P2pEventHandler
 * P2P Event Handler
 */
class P2pEventHandler : public boost::noncopyable
{
public:
    virtual ~P2pEventHandler() {}

    /**
     * New peer connected.
     * @param peerId peer ID
     */
    virtual void onPeerConnected(PeerId peerId) = 0;

    /**
     * A peer disconnected.
     * @param peerId peer ID
     */
    virtual void onPeerDisconnected(PeerId peerId) = 0;

    /**
     * Failed in connecting a peer
     * @param peerId peer ID.
     *        If the peer is the host, this value is @c pseudoPeerId.
     */
    virtual void onConnectFailed(PeerId peerId) = 0;

    /**
     * Resolved public(external) UDP/IP address.
     * @param ipAddress public(external) UDP/IP address
     * @param port public or NAT mapped port number
     * - called periodically.
     */
    virtual void onAddressResolved(const srpc::String& ipAddress,
        srpc::UInt16 port) = 0;

    /**
     * Host migrated.
     * @param peerId New host's PeerId
     */
    virtual void onHostMigrated(PeerId peerId) = 0;

    /**
     * A group is created by other peer
     */
    virtual void onGroupCreated(const RGroupInfo& groupInfo) = 0;

    /**
     * A peer joined a group
     */
    virtual void onGroupJoined(GroupId groupId, PeerId peerId) = 0;

    /**
     * A peer left from a group
     */
    virtual void onGroupLeft(GroupId groupId, PeerId peerId) = 0;
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_P2PEVENTHANDLER_H
