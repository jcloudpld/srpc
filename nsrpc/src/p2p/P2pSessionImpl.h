#ifndef NSRPC_P2PSESSIONIMPL_H
#define NSRPC_P2PSESSIONIMPL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "SystemServiceImpl.h"
#include "StunServiceImpl.h"
#include "RelayServiceImpl.h"
#include "PeerCandidate.h"
#include "PeerManager.h"
#include "GroupManager.h"
#include "PluginManager.h"
#include "PeerMessageHandler.h"
#include "RelayServiceHandler.h"
#include "StunServiceHandler.h"
#include "SystemServiceHandler.h"
#include "AnonymousMessageManager.h"
#include <nsrpc/p2p/detail/PeerCipherKeyCache.h>
#include <nsrpc/p2p/detail/P2pEndpoint.h>
#include <nsrpc/p2p/detail/P2pRpcNetwork.h>
#include <nsrpc/p2p/detail/PeerNetworkSender.h>
#include <nsrpc/p2p/detail/PeerNotifier.h>
#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/p2p/P2pConfig.h>
#include <nsrpc/p2p/P2pSession.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/utility/MessageBlockManager.h>

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class P2pSessionImpl
 * P2P Session Impl
 */
class P2pSessionImpl : public P2pSession,
    private PeerNotifier,
    private PeerNetworkSender,
    private PeerMessageHandler,
    private RelayServiceHandler,
    private StunServiceHandler,
    private SystemServiceHandler,
    private P2pEventHandler
{
public:
    P2pSessionImpl(PeerId peerId, P2pEventHandler& eventHandler,
        const P2pConfig& p2pConfig, bool useBitPacking,
        PacketCoder* packetCoder);
    virtual ~P2pSessionImpl();
private: // information hiding
    virtual void attach(PlugInPtr& plugIn);
    virtual void detach(PlugInPtr& plugIn);

    virtual bool open(srpc::UInt16 port, const srpc::String& password,
        P2pOptions p2pOptions);
    virtual void close();

    virtual void addP2pOptions(P2pOptions p2pOptions);

    virtual void host(size_t maxPeers, bool hostMigration,
        const PeerIds& hostPrecedence);
    virtual void connect(const PeerAddresses& hostAddresses);
    virtual void disconnect();

    virtual void disconnect(PeerId peerId);

    virtual void setRelayServer(const PeerAddress& address,
        const srpc::String& cipherKey);

    virtual void setHost(PeerId newHostId);

    virtual void tick();

    virtual GroupId createGroup(const RGroupName& groupName);
    virtual bool destroyGroup(GroupId groupId);
    virtual bool joinGroup(GroupId groupId);
    virtual bool leaveGroup(GroupId groupId);

    virtual const RGroupMap& getGroups() const;

    virtual bool isHost() const {
        return peerManager_.isHost();
    }
    virtual size_t getPeerCount() const {
        return peerManager_.getPeerCount();
    }
    virtual PeerId getPeerId() const {
        return myPeerId_;
    }

    virtual PeerAddress getTargetAddress(PeerId peerId) const;

    virtual PeerAddresses getAddresses(PeerId peerId) const;

    virtual P2pOptions getP2pOptions(PeerId peerId) const;

    virtual PeerStats getStats(PeerId peerId) const;
    virtual srpc::String getStatsString(PeerId peerId) const;

    virtual srpc::RpcNetwork& getRpcNetwork() {
        return rpcNetwork_;
    }

    virtual PeerId getHostPeerId() const;
    virtual bool isHostAlive() const;
private:
    void connectToPeer(PeerId peerId, const Addresses& peerAddresses);
    void makeHost(PeerId peerId);

    void detectConnectionTimeout();

    void addMyPeer(P2pOptions p2pOptions);
    void flush();
    void resolve();
    void tryToConnect(PeerId peerId,
        const ACE_INET_Addr& targetAddress, const ACE_INET_Addr& peerAddress);

    bool encodeMessage(ACE_Message_Block& mblock,
        const PeerIdPair& peerIdPair, srpc::RpcPacketType packetType,
        SequenceNumber sequenceNumber, PeerTime sentTime);
    bool decodeMessage(P2pPacketHeader& header,
        const ACE_INET_Addr& peerAddress, ACE_Message_Block& mblock);

    void disconnected(PeerId peerId);
    void messageArrived(const P2pPacketHeader& header,
        const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock);

    void acknowledge(PeerId peerId, const Message& message);

    void migrateHost();

    bool canRelay(const ACE_INET_Addr& targetAddress) const;
    bool isRelayServerAddress(const ACE_INET_Addr& address) const {
        return address == relayServerAddress_;
    }
    bool isSelf(PeerId peerId) const {
        return peerId == myPeerId_;
    }
private:
    // = PeerNotifier overriding
    virtual void onMessageArrived(const ACE_INET_Addr& peerAddress);

    // = PeerNetworkSender overriding
    virtual bool sendNow(const PeerIdPair& peerIdPair,
        const AddressPair& addressPair, const ACE_Message_Block& mblock,
        srpc::RpcPacketType packetType, SequenceNumber sequenceNumber,
        PeerTime sentTime);
    virtual void marshalingErrorOccurred();
    virtual void sendOutgoingMessage(srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, const P2pPeerHint* peerHint);
    virtual size_t getPacketHeaderSize() const;

    // = PeerMessageHandler overriding
    virtual bool handleIncomingMessage(PeerId peerId,
        const Message& message);
    virtual bool acknowledgedConnect(PeerId peerId);
    virtual void peerDisconnecting(PeerId peerId);
    virtual void sendPing(PeerId peerId);
    virtual void sendAcknowledgement(PeerId peerId, const Message& message);

    // = SystemServiceHandler overriding
    virtual bool authenticate(PeerId peerId,
        const srpc::String& sessionPassword, srpc::UInt32 sessionKey);
    virtual bool peerConnected(PeerId peerId,
        const ACE_INET_Addr& targetAddress, const RAddresses& peerAddresses,
        P2pOptions p2pOptions);
    virtual void peerDisconnected(PeerId peerId);
    virtual void connectToNewPeer(PeerId peerId,
        const Addresses& peerAddresses);
    virtual void hostMigrated(PeerId peerId);
    virtual void setGroups(const RGroupMap& groups);
    virtual void groupCreated(const RGroupInfo& groupInfo);
    virtual void groupDestroyed(GroupId groupId);
    virtual void groupJoined(GroupId groupId, PeerId peerId);
    virtual void groupLeft(GroupId groupId, PeerId peerId);
    virtual void setP2pProperty(const RP2pProperty& p2pProperty) {
        p2pProperty_ = p2pProperty;
    }
    virtual bool shouldConnectReversal(const ACE_INET_Addr& address) const {
        return isRelayServerAddress(address);
    }
    virtual bool isHostConnected() const;
    virtual const RGroupMap& getCurrentGroups() const;

    // = StunServiceHandler overriding
    virtual void resolved(const srpc::String& ipAddress, srpc::UInt16 port);

    // = RelayServiceHandler overriding
    virtual ACE_Message_Block* acquire(size_t blockSize);
    virtual void relayed(PeerId peerId, const ACE_INET_Addr& peerAddress,
        ACE_Message_Block* mblock, srpc::RpcPacketType packetType,
        SequenceNumber sequenceNumber, srpc::UInt32 sentTime);

    // = P2pEventHandler overriding
    virtual void onPeerConnected(PeerId peerId);
    virtual void onPeerDisconnected(PeerId peerId);
    virtual void onConnectFailed(PeerId peerId);
    virtual void onAddressResolved(const srpc::String& ipAddress,
        srpc::UInt16 port);
    virtual void onHostMigrated(PeerId peerId);
    virtual void onGroupCreated(const RGroupInfo& groupInfo);
    virtual void onGroupDestroyed(GroupId groupId);
    virtual void onGroupJoined(GroupId groupId, PeerId peerId);
    virtual void onGroupLeft(GroupId groupId, PeerId peerId);

private:
    const PeerId myPeerId_; ///< my peer id
    P2pEventHandler& eventHandler_;
    const P2pConfig p2pConfig_;
    boost::scoped_ptr<ACE_Reactor> reactor_;

    boost::scoped_ptr<PacketCoder> packetCoder_;
    NoSynchMessageBlockManager messageBlockManager_;
    ACE_Message_Block* recvBlock_;
    ACE_Message_Block* sendBlock_;

    P2pRpcNetwork rpcNetwork_;
    P2pEndpoint endpoint_;

    RpcSystemServiceImpl systemService_;
    RpcStunServiceImpl stunService_;
    RpcRelayServiceImpl relayService_;

    AnonymousMessageManager anonymousMessageManager_;

    ACE_INET_Addr relayServerAddress_;

    RP2pProperty p2pProperty_;

    PacketCoder::Seed encryptSeed_;
    PacketCoder::Seed decryptSeed_;

    PeerCipherKeyCache peerCipherKeys_;

    PeerCandidateManager peerCandidateManager_;
    PeerManager peerManager_;
    GroupManager groupManager_;

    PluginManager plugInManager_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_P2PSESSIONIMPL_H
