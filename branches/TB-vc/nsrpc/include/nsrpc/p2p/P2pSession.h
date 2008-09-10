#ifndef NSRPC_P2PSESSION_H
#define NSRPC_P2PSESSION_H

#include "PeerId.h"
#include "PeerStats.h"
#include "PeerAddress.h"
#include "Group.h"
#include <boost/noncopyable.hpp>

class ACE_Reactor;

namespace srpc
{
class RpcNetwork;
} // namespace srpc

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/**
 * @class P2pSession
 * The session interface for a P2P.
 * @warning no thread-safe
 */
class P2pSession : public boost::noncopyable
{
public:
    virtual ~P2pSession() {}

    /**
     * Initialize this session.
     * @param port local listening port
     * @param password P2P session password
     */
    virtual bool open(srpc::UInt16 port, const srpc::String& password = "") = 0;

    /// Disconnect & Deinitialize this session.
    virtual void close() = 0;

    /**
     * Creates a new P2P session, hosted by the local computer.
     * @param maxPeers the maximum number of peers allowed in the session.
     *        Set this member to 0 to specify an unlimited number of players.
     * @param hostMigration enable host migration?
     * @param hostPrecedence if not empty, host migration is occurred by this order
     */
    virtual void host(size_t maxPeers = 0, bool hostMigration = true,
        const PeerIds& hostPrecedence = PeerIds()) = 0;

    /**
     * Establishes the connection to all the peers in a peer-to-peer session.
     * @param hostAddresses the addresses to use to connect to the P2P host.
     */
    virtual void connect(const PeerAddresses& hostAddresses) = 0;

    /// disconnect all peers from the current session
    virtual void disconnect() = 0;

    /// disconnect the peer from the current session
    virtual void disconnect(PeerId peerId) = 0;

    /**
     * Set the UDP Relay(STUN) server.
     * @param address Relay server's address
     * @param cipherKey relay server's packet cipher key.
     *                  If the key is null string, a default key is used.
     */
    virtual void setRelayServer(const PeerAddress& address,
        const srpc::String& cipherKey = "") = 0;

    /**
     * tick. (handle events, etc)
     * - tick() should be called fairly regularly for adequate performance.
     */
    virtual void tick() = 0;

    /**
     * create a group (only host allowed)
     * @param groupName group name (duplication allowed)
     */
    virtual GroupId createGroup(const RGroupName& groupName) = 0;

    /// join a group
    virtual bool joinGroup(GroupId groupId) = 0;

    /**
     * get P2P groups
     */
    virtual const RGroupMap& getGroups() const = 0;

    /// Is the host for a P2P session.
    virtual bool isHost() const = 0;

    /// Get the number of Peers that connected.
    virtual size_t getPeerCount() const = 0;

    /// Get my PeerId.
    virtual PeerId getPeerId() const = 0;

    /// Get a peer's target address(connection path).
    virtual PeerAddress getTargetAddress(PeerId peerId) const = 0;

    /**
     * Get the addresses of the peer.
     * - If the address resolved via RelayServer, first address is public.
     */
    virtual PeerAddresses getAddresses(PeerId peerId) const = 0;

    /// Get the statistics of the peer.
    virtual PeerStats getStats(PeerId peerId) const = 0;

    /// Get the statistics string of the peer.
    virtual std::string getStatsString(PeerId peerId) const = 0;

    /// get the RpcNetwork instance for RPC bind.
    virtual srpc::RpcNetwork& getRpcNetwork() = 0;

    /// Get host PeerId.
    virtual PeerId getHostPeerId() const = 0;

    /// is host alive?
    virtual bool isHostAlive() const = 0;
};

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_P2PSESSION_H
