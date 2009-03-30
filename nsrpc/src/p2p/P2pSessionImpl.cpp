#include "stdafx.h"
#include "P2pSessionImpl.h"
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
#include <srpc/srpc_macros.h>
#include <ace/Reactor.h>
#include <cassert>

namespace nsrpc
{

namespace detail
{

namespace
{

InitAce initAce;

} // namespace

// = P2pSessionImpl

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

P2pSessionImpl::P2pSessionImpl(PeerId peerId, P2pEventHandler& eventHandler,
    const P2pConfig& p2pConfig, bool useBitPacking,
    PacketCoder* packetCoder) :
    myPeerId_(peerId),
    eventHandler_(eventHandler),
    p2pConfig_(p2pConfig),
    reactor_(new ACE_Reactor),
    packetCoder_(packetCoder),
    messageBlockManager_(packetCoder_->getDefaultPacketPoolSize(),
        packetCoder_->getDefaultPacketSize()),
    recvBlock_(messageBlockManager_.create(
        packetCoder_->getDefaultPacketSize())),
    sendBlock_(messageBlockManager_.create(
        packetCoder_->getDefaultPacketSize())),
    rpcNetwork_(*this, *recvBlock_, *sendBlock_, useBitPacking),
    endpoint_(*this, *recvBlock_, reactor_.get()),
    systemService_(peerManager_, *this, *this, rpcNetwork_),
    stunService_(*this, rpcNetwork_),
    relayService_(*this, rpcNetwork_),
    anonymousMessageManager_(*this),
    peerCipherKeys_(*packetCoder_),
    peerCandidateManager_(*this, p2pConfig_, myPeerId_),
    peerManager_(*this, *this, p2pConfig_, groupManager_),
    groupManager_(systemService_)
{
    assert(isAllowedPeerId(peerId));
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

P2pSessionImpl::~P2pSessionImpl()
{
    close();
    recvBlock_->release();
    sendBlock_->release();
}


void P2pSessionImpl::attach(PlugInPtr& plugIn)
{
    plugInManager_.add(plugIn);
    plugIn->attached(this);
}


void P2pSessionImpl::detach(PlugInPtr& plugIn)
{
    plugInManager_.remove(plugIn);
    plugIn->detached();
}


bool P2pSessionImpl::open(srpc::UInt16 port,
    const srpc::String& password, P2pOptions p2pOptions)
{
    if (! isAllowedPeerId(myPeerId_)) {
        return false;
    }

    if (! endpoint_.isOpened()) {
        if (! endpoint_.open(port)) {
            return false;
        }
    }

    resetBaseTime();
    setPeerTime();

    addMyPeer(p2pOptions);

    p2pProperty_.sessionPassword_ = password;

    packetCoder_->setDecryptSeed(peerCipherKeys_.get(myPeerId_));

    return true;
}


void P2pSessionImpl::close()
{
    disconnect();

    endpoint_.close();
}


void P2pSessionImpl::addP2pOptions(P2pOptions p2pOptions)
{
    const PeerPtr me(peerManager_.getMe());
    if (me.isNull()) {
        assert(false && "must call after open()");
        return;
    }

    me->addP2pOptions(p2pOptions);
}


void P2pSessionImpl::host(size_t maxPeers, bool hostMigration,
    const PeerIds& hostPrecedence)
{
    assert(endpoint_.isOpened());
    assert(! isHost());

    p2pProperty_.maxPeers_ = static_cast<srpc::UInt8>(maxPeers);
    p2pProperty_.hostMigration_ = hostMigration;
    p2pProperty_.hostPrecedence_.assign(
        hostPrecedence.begin(), hostPrecedence.end());

    makeHost(myPeerId_);
}


void P2pSessionImpl::connect(const PeerAddresses& hostAddresses)
{
    assert(endpoint_.isOpened());
    assert(! isHost());

    peerCandidateManager_.reset();
    connectToPeer(pseudoHostPeerId, toAddresses(hostAddresses));
}


void P2pSessionImpl::disconnect()
{
    const PeerPtr me(peerManager_.getMe());
    if ((! me.isNull()) && me->disconnect()) {
        systemService_.rpcDisconnect();
        flush();
    }

    anonymousMessageManager_.reset();
    peerCandidateManager_.reset();
    peerManager_.reset();
    groupManager_.reset();

    packetCoder_->reset();

    p2pProperty_.reset();

    systemService_.reset();
}


void P2pSessionImpl::disconnect(PeerId peerId)
{
    peerManager_.removePeerNextTime(peerId);
}


void P2pSessionImpl::setRelayServer(const PeerAddress& address,
    const srpc::String& cipherKey)
{
    peerManager_.addRelayServer(address);
    relayServerAddress_ = peerManager_.getRelayServer()->getTargetAddress();

    if (! cipherKey.empty()) {
        peerCipherKeys_.set(relayServerPeerId, cipherKey);
    }

    stunService_.relayServerSetUp();
}


void P2pSessionImpl::setHost(PeerId newHostId)
{
    hostMigrated(newHostId);
}


void P2pSessionImpl::tick()
{
    setPeerTime();

    resolve();

    detectConnectionTimeout();

#ifdef USE_P2P_POLLING
    endpoint_.read();
#else
    reactor_->handle_events(
        const_cast<ACE_Time_Value&>(ACE_Time_Value::zero));
#endif

    anonymousMessageManager_.handleIncomingMessages();
    peerManager_.handleIncomingMessages();

    peerManager_.sendOutgoingMessages();
    peerManager_.handleDisconnectedPeers();

    plugInManager_.update();
}


GroupId P2pSessionImpl::createGroup(const RGroupName& groupName)
{
    if (! isHost()) {
        return giUnknown;
    }

    return groupManager_.createGroup(groupName);
}


bool P2pSessionImpl::destroyGroup(GroupId groupId)
{
    if (! isHost()) {
        return false;
    }

    return groupManager_.destroyGroup(groupId);
}


bool P2pSessionImpl::joinGroup(GroupId groupId)
{
    assert(isValid(groupId));

    return groupManager_.joinGroup(groupId, myPeerId_);
}


bool P2pSessionImpl::leaveGroup(GroupId groupId)
{
    assert(isValid(groupId));

    return groupManager_.leaveGroup(groupId, myPeerId_);
}


const RGroupMap& P2pSessionImpl::getGroups() const
{
    return groupManager_.getGroups();
}


PeerAddress P2pSessionImpl::getTargetAddress(PeerId peerId) const
{
    const PeerPtr peer(peerManager_.getPeer(peerId));
    if (! peer.isNull()) {
        return toPeerAddress(peer->getTargetAddress());
    }
    return PeerAddress();
}


PeerAddresses P2pSessionImpl::getAddresses(PeerId peerId) const
{
    const PeerPtr peer(peerManager_.getPeer(peerId));
    if (! peer.isNull()) {
        return toPeerAddresses(peer->getPeerAddresses());
    }
    return PeerAddresses();
}


P2pOptions P2pSessionImpl::getP2pOptions(PeerId peerId) const
{
    const PeerPtr peer(peerManager_.getPeer(peerId));
    if (! peer.isNull()) {
        return peer->getP2pOptions();
    }
    return poNone;
}


PeerStats P2pSessionImpl::getStats(PeerId peerId) const
{
    const PeerPtr peer(peerManager_.getPeer(peerId));
    if (! peer.isNull()) {
        return peer->getStats();
    }
    return PeerStats();
}


std::string P2pSessionImpl::getStatsString(PeerId peerId) const
{
    const PeerPtr peer(peerManager_.getPeer(peerId));
    if (! peer.isNull()) {
        return peer->getStatsString();
    }
    return "";
}


PeerId P2pSessionImpl::getHostPeerId() const
{
    return peerManager_.getHostPeerId();
}


bool P2pSessionImpl::isHostAlive() const
{
    return peerManager_.isHostAlive();
}


void P2pSessionImpl::addMyPeer(P2pOptions p2pOptions)
{
    assert(! endpoint_.getLocalAddresses().empty());
    peerManager_.addPeer(myPeerId_, endpoint_.getLocalAddresses(),
        p2pOptions);
    peerManager_.getMe()->connected();
}


void P2pSessionImpl::flush()
{
    peerManager_.sendOutgoingMessages();
}


void P2pSessionImpl::connectToPeer(PeerId peerId,
    const Addresses& peerAddresses)
{
    assert(! isSelf(peerId));

    if (peerManager_.isExists(peerId)) {
        return;
    }

    Addresses::const_iterator pos = peerAddresses.begin();
    const Addresses::const_iterator end = peerAddresses.end();
    for (; pos != end; ++pos) {
        const ACE_INET_Addr& address = *pos;
        tryToConnect(peerId, address, address);
        if (canRelay(address)) {
            tryToConnect(peerId, relayServerAddress_, address);
        }
    }
}


void P2pSessionImpl::makeHost(PeerId peerId)
{
    peerManager_.setHost(peerId);
    if (peerId == myPeerId_) {
        p2pProperty_.sessionKey_ = (rand() << 16) | rand();
        systemService_.setHost(p2pProperty_);
    }
}


void P2pSessionImpl::detectConnectionTimeout()
{
    const PeerIds failedPeerIds =
        peerCandidateManager_.detectConnectionTimeout();

    PeerIds::const_iterator pos = failedPeerIds.begin();
    const PeerIds::const_iterator end = failedPeerIds.end();
    for (; pos != end; ++pos) {
        const PeerId peerId = *pos;
        if (! peerManager_.isExists(peerId)) {
            onConnectFailed(peerId);
        }
    }
}


void P2pSessionImpl::resolve()
{
    if (! peerManager_.isExists(relayServerPeerId)) {
        return;
    }

    stunService_.resolve();
}


void P2pSessionImpl::tryToConnect(PeerId peerId,
    const ACE_INET_Addr& targetAddress, const ACE_INET_Addr& peerAddress)
{
    assert(! peerManager_.isExists(peerId));

    peerCandidateManager_.addCandidate(peerId,
        AddressPair(targetAddress, peerAddress));
    const P2pPeerHint hint(peerId, &targetAddress, true);
    const PeerPtr me(peerManager_.getMe());
    systemService_.rpcConnect(me->getPeerAddresses(), me->getP2pOptions(),
        p2pProperty_.sessionPassword_, p2pProperty_.sessionKey_, &hint);

    NSRPC_LOG_DEBUG4(ACE_TEXT("Try to connect(P%u) via (%s:%d)"),
        peerId,
        targetAddress.get_host_addr(), targetAddress.get_port_number());
}


bool P2pSessionImpl::canRelay(const ACE_INET_Addr& targetAddress) const
{
#ifdef DO_NOT_ALLOW_LOCAL_RELAY
    if (! isPublicAddress(targetAddress)) {
        return false;
    }
#else
    SRPC_UNUSED_ARG(targetAddress);
#endif

    return peerManager_.isExists(relayServerPeerId);
}


void P2pSessionImpl::messageArrived(const P2pPacketHeader& header,
    const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock)
{
    if (isSelf(header.peerId_)) {
        mblock->release();
        return;
    }

    if (! peerManager_.isExists(header.peerId_)) {
        anonymousMessageManager_.putIncomingMessage(header, peerAddress,
            mblock);
    }
    else {
        peerManager_.putIncomingMessage(header, peerAddress, mblock);
    }
}


void P2pSessionImpl::acknowledge(PeerId peerId, const Message& message)
{
    if (isSelf(peerId)) {
        return;
    }

    if (! peerManager_.canSendAcknowledgement(peerId)) {
        return;
    }

    const P2pPeerHint hint(peerId);
    systemService_.rpcAcknowledgement(message.sequenceNumber_,
        message.sentTime_, &hint);
}


void P2pSessionImpl::disconnected(PeerId peerId)
{
    if (! peerManager_.isExists(peerId)) {
        return;
    }

    onPeerDisconnected(peerId);

    if (peerManager_.isHost(peerId) || (! peerManager_.isHostExists())) {
        migrateHost();
    }
}


void P2pSessionImpl::migrateHost()
{
    if (p2pProperty_.hostMigration_ &&
        peerManager_.isHostCandidate(p2pProperty_.hostPrecedence_)) {
        systemService_.rpcHostMigrated();

        hostMigrated(myPeerId_);
    }
}


bool P2pSessionImpl::encodeMessage(ACE_Message_Block& mblock,
    const PeerIdPair& peerIdPair, srpc::RpcPacketType packetType,
    SequenceNumber sequenceNumber, PeerTime sentTime)
{
    P2pPacketHeader header(packetType, peerIdPair.from_, sequenceNumber,
        sentTime);
    packetCoder_->setEncryptSeed(peerCipherKeys_.get(peerIdPair.to_));
    if (! packetCoder_->encode(mblock, header)) {
        NSRPC_LOG_ERROR(ACE_TEXT("PacketCoder::encode() FAILED!!"));
        return false;
    }

    return true;
}


bool P2pSessionImpl::decodeMessage(P2pPacketHeader& header,
    const ACE_INET_Addr& peerAddress, ACE_Message_Block& mblock)
{
    if (! packetCoder_->readHeader(header, mblock)) {
        NSRPC_LOG_ERROR3(
            ACE_TEXT("P2pSessionImpl::decodeMessage(from: %s:%d) ")
            ACE_TEXT("FAILED!!!(Invalid Message Header)"),
            peerAddress.get_host_addr(), peerAddress.get_port_number());
        return false;
    }

    if (! packetCoder_->isValidHeader(header, mblock)) {
        NSRPC_LOG_ERROR3(
            ACE_TEXT("P2pSessionImpl::decodeMessage(from: %s:%d) ")
            ACE_TEXT("FAILED!!!(Message body is too short)"),
            peerAddress.get_host_addr(), peerAddress.get_port_number());
        return false;
    }

    packetCoder_->setDecryptSeed(peerCipherKeys_.get(myPeerId_));
    if (! packetCoder_->decode(mblock)) {
        if (! isHost()) {
            return false;
        }

        packetCoder_->setDecryptSeed(peerCipherKeys_.get(pseudoHostPeerId));
        if (! packetCoder_->decode(mblock)) {
            return false;
        }
    }

    packetCoder_->advanceToBody(mblock);
    return true;
}

// = overriding

void P2pSessionImpl::onMessageArrived(const ACE_INET_Addr& peerAddress)
{
    P2pPacketHeader header;
    if (! decodeMessage(header, peerAddress, *recvBlock_)) {
        return;
    }

    messageArrived(header, peerAddress, recvBlock_->clone());
}


bool P2pSessionImpl::sendNow(const PeerIdPair& peerIdPair,
    const AddressPair& addressPair, const ACE_Message_Block& mblock,
    srpc::RpcPacketType packetType, SequenceNumber sequenceNumber,
    PeerTime sentTime)
{
    //NSRPC_LOG_DEBUG8("P2pSessionImpl::sendNow(P%u, #%d, %d, %s:%d, %s, %u)",
    //    peerIdPair.to_, sequenceNumber, sentTime,
    //    addressPair.targetAddress_.get_host_addr(),
    //    addressPair.targetAddress_.get_port_number(),
    //    isReliable(packetType) ? "reliable" : "unreliable",
    //    getPeerTime());

    if (isRelayServer(peerIdPair.to_) ||
        (! isRelayServerAddress(addressPair.targetAddress_))) {
        AceMessageBlockGuard mblockGuard(mblock.clone());
        if (! encodeMessage(*mblockGuard, peerIdPair, packetType,
            sequenceNumber, sentTime)) {
            return false;
        }

        if (! endpoint_.send(addressPair.targetAddress_, *mblockGuard)) {
            return false;
        }
    }
    else {
        const P2pPeerHint hint(relayServerPeerId);
        const size_t headerSize = packetCoder_->getHeaderSize();
        relayService_.rpcRelay(peerIdPair, addressPair.peerAddress_,
            RMessageBuffer(mblock.rd_ptr() + headerSize,
                static_cast<srpc::UInt16>(mblock.length() - headerSize)),
            packetType, sequenceNumber, sentTime, &hint);
    }
    return true;
}


void P2pSessionImpl::marshalingErrorOccurred()
{
    endpoint_.close();
}


void P2pSessionImpl::sendOutgoingMessage(srpc::RpcPacketType packetType,
    ACE_Message_Block* mblock, const P2pPeerHint* peerHint)
{
    PeerId peerId = invalidPeerId;
    GroupId groupId = giUnknown;
    ACE_INET_Addr targetAddress;
    P2pOptions p2pOptions = poNone;
    if (peerHint != 0) {
        peerId = peerHint->peerId_;
        groupId = peerHint->groupId_;
        targetAddress = peerHint->getAddress();
        p2pOptions = peerHint->p2pOptions_;
        if (peerHint->isCandidate_) {
            if (peerCandidateManager_.putOutgoingMessage(peerId,
                targetAddress, mblock)) {
                assert(! isValid(groupId));
                return;
            }
        }
    }

    if (isValid(groupId)) {
        peerManager_.putOutgoingMessage(groupId, targetAddress, packetType,
            mblock, p2pOptions);
    }
    else {
        if ((! isSelf(peerId)) &&
            (peerManager_.isExists(peerId) || isBroadcast(peerId))) {
            peerManager_.putOutgoingMessage(peerId, targetAddress, packetType,
                mblock, p2pOptions);
        }
        else {
            //NSRPC_LOG_DEBUG3(
            //    ACE_TEXT("P2pSessionImpl::sendOutgoingMessage(P%u,%d)")
            //    ACE_TEXT("- Target missed"),
            //    peerId, packetType);
        }
    }
}


size_t P2pSessionImpl::getPacketHeaderSize() const
{
    return packetCoder_->getHeaderSize();
}


// CAUTION: 에러 발생시 패킷 헤더가 잘못될 수 있으므로 접속 종료하지 않는다.
bool P2pSessionImpl::handleIncomingMessage(PeerId peerId,
    const Message& message)
{
    recvBlock_->reset();
    recvBlock_->copy(message.mblock_->rd_ptr(), message.mblock_->length());
    if (! rpcNetwork_.handleMessage(peerId, message.peerAddress_)) {
        NSRPC_LOG_DEBUG3(
            ACE_TEXT("P2pSessionImpl::handleIncomingMessage(P%u,#%u) FAILED."),
            peerId, message.sequenceNumber_);
        return false;
    }

    return true;
}


bool P2pSessionImpl::acknowledgedConnect(PeerId peerId)
{
    const PeerPtr peer(peerManager_.getPeer(peerId));
    if (peer.isNull()) {
        return false;
    }
    
    peer->acknowledgeConnect();

    onPeerConnected(peerId);

    return true;
}


void P2pSessionImpl::peerDisconnecting(PeerId peerId)
{
    disconnected(peerId);
}


void P2pSessionImpl::sendPing(PeerId peerId)
{
    const P2pPeerHint hint(peerId);
    systemService_.rpcPing(&hint);
}


void P2pSessionImpl::sendAcknowledgement(PeerId peerId,
    const Message& message)
{
    acknowledge(peerId, message);
}


bool P2pSessionImpl::authenticate(PeerId peerId,
    const srpc::String& sessionPassword, srpc::UInt32 sessionKey)
{
    if (! peerManager_.isHost()) {
        if (p2pProperty_.sessionKey_ != 0) {
            if (sessionKey != p2pProperty_.sessionKey_) {
                return false;
            }
        }
    }

    if (sessionPassword != p2pProperty_.sessionPassword_) {
        return false;
    }

    if (p2pProperty_.maxPeers_ > 0) {
        if ((! peerManager_.isExists(peerId)) &&
            (peerManager_.getPeerCount() >= p2pProperty_.maxPeers_)) {
            return false;
        }
    }

    return true;
}


bool P2pSessionImpl::peerConnected(PeerId peerId,
    const ACE_INET_Addr& targetAddress, const RAddresses& peerAddresses,
    P2pOptions p2pOptions)
{
    bool firstConnection = false;
    bool connected = false;

    if (! peerManager_.isExists(peerId)) {
        PeerId removingPeerId = peerId;
        if (! peerManager_.isHostConnected()) {
            removingPeerId = pseudoHostPeerId;
        }
        peerCandidateManager_.removeCandidates(removingPeerId);

        const Addresses addresses = toAddresses(peerAddresses);
        assert(! addresses.empty());
        peerManager_.addPeer(peerId, addresses, p2pOptions);
        connected = true;
        firstConnection = true;
    }
    else if (! isRelayServerAddress(targetAddress)) {
        connected = true;
    }

    if (connected) {
        const PeerPtr peer(peerManager_.getPeer(peerId));
        peer->setTargetAddress(targetAddress);
        NSRPC_LOG_DEBUG6(ACE_TEXT("Peer(P%u,0x%X) %sconnected via %s:%d."),
            peerId, p2pOptions, (firstConnection ? "" : "re"),
            targetAddress.get_host_addr(),
            targetAddress.get_port_number());
    }

    return firstConnection;
}


void P2pSessionImpl::peerDisconnected(PeerId peerId)
{
    //disconnected(peerId);
    peerManager_.removePeerNextTime(peerId);
}


void P2pSessionImpl::connectToNewPeer(PeerId peerId,
    const Addresses& peerAddresses)
{
    connectToPeer(peerId, peerAddresses);
}


void P2pSessionImpl::hostMigrated(PeerId peerId)
{
    makeHost(peerId);

    onHostMigrated(peerId);
}


void P2pSessionImpl::setGroups(const RGroupMap& groups)
{
    groupManager_.set(groups);
}


void P2pSessionImpl::groupCreated(const RGroupInfo& groupInfo)
{
    groupManager_.groupCreated(groupInfo);

    onGroupCreated(groupInfo);
}


void P2pSessionImpl::groupDestroyed(GroupId groupId)
{
    groupManager_.groupDestroyed(groupId);

    onGroupDestroyed(groupId);
}


void P2pSessionImpl::groupJoined(GroupId groupId, PeerId peerId)
{
    groupManager_.groupJoined(groupId, peerId);

    onGroupJoined(groupId, peerId);
}


void P2pSessionImpl::groupLeft(GroupId groupId, PeerId peerId)
{
    groupManager_.groupLeft(groupId, peerId);

    onGroupLeft(groupId, peerId);
}


bool P2pSessionImpl::isHostConnected() const
{
    return (! isHost()) && peerManager_.isHostConnected();
}


const RGroupMap& P2pSessionImpl::getCurrentGroups() const
{
    return groupManager_.getGroups();
}


void P2pSessionImpl::resolved(const srpc::String& ipAddress, srpc::UInt16 port)
{
    const PeerPtr me(peerManager_.getMe());
    if (me.isNull()) {
        return;
    }

    const ACE_INET_Addr resolvedAddress(port, ipAddress.c_str());
    me->setResolvedAddress(resolvedAddress);
    onAddressResolved(ipAddress, port);
}


ACE_Message_Block* P2pSessionImpl::acquire(size_t blockSize)
{
    return messageBlockManager_.create(blockSize);
}


void P2pSessionImpl::relayed(PeerId peerId,
    const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock,
    srpc::RpcPacketType packetType, SequenceNumber sequenceNumber,
    srpc::UInt32 sentTime)
{
    const P2pPacketHeader pseudoHeader(packetType, peerId, sequenceNumber,
        sentTime);

    messageArrived(pseudoHeader, peerAddress, mblock);
}

// = P2pEventHandler overriding

void P2pSessionImpl::onPeerConnected(PeerId peerId)
{
    plugInManager_.peerConnected(peerId);

    eventHandler_.onPeerConnected(peerId);
}


void P2pSessionImpl::onPeerDisconnected(PeerId peerId)
{
    plugInManager_.peerDisconnected(peerId);

    eventHandler_.onPeerDisconnected(peerId);
}


void P2pSessionImpl::onConnectFailed(PeerId peerId)
{
    eventHandler_.onConnectFailed(peerId);
}


void P2pSessionImpl::onAddressResolved(const srpc::String& ipAddress,
    srpc::UInt16 port)
{
    eventHandler_.onAddressResolved(ipAddress, port);
}


void P2pSessionImpl::onHostMigrated(PeerId peerId)
{
    eventHandler_.onHostMigrated(peerId);
}


void P2pSessionImpl::onGroupCreated(const RGroupInfo& groupInfo)
{
    eventHandler_.onGroupCreated(groupInfo);
}


void P2pSessionImpl::onGroupDestroyed(GroupId groupId)
{
    eventHandler_.onGroupDestroyed(groupId);
}


void P2pSessionImpl::onGroupJoined(GroupId groupId, PeerId peerId)
{
    eventHandler_.onGroupJoined(groupId, peerId);
}


void P2pSessionImpl::onGroupLeft(GroupId groupId, PeerId peerId)
{
    eventHandler_.onGroupLeft(groupId, peerId);
}

} // namespace detail

} // namespace nsrpc
