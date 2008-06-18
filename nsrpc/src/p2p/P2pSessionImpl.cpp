#include "stdafx.h"
#include "P2pSessionImpl.h"
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
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
    peerCandidateManager_(*this, p2pConfig_, myPeerId_),
    peerManager_(*this, *this, p2pConfig_),
    rpcNetwork_(*this, *recvBlock_, *sendBlock_, useBitPacking),
    endpoint_(*this, *recvBlock_, reactor_.get()),
    systemService_(peerManager_, eventHandler, *this, rpcNetwork_),
    stunService_(*this, rpcNetwork_),
    relayService_(*this, rpcNetwork_),
    anonymousMessageManager_(*this),
    peerCipherKeys_(*packetCoder_)
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


bool P2pSessionImpl::open(srpc::UInt16 port,
    const srpc::String& password)
{
    //if (endpoint_.isOpened()) {
    //    return true;
    //}
    close();

    if (! isAllowedPeerId(myPeerId_)) {
        return false;
    }

    if (! endpoint_.open(port)) {
        return false;
    }

    resetBaseTime();
    setPeerTime();

    addMyPeer();

    p2pProperty_.sessionPassword_ = password;

    packetCoder_->setDecryptSeed(peerCipherKeys_.get(myPeerId_));

    return true;
}


void P2pSessionImpl::close()
{
    const PeerPtr me(peerManager_.getMe());
    if ((! me.isNull()) && me->disconnect()) {
        systemService_.rpcDisconnect();
        flush();
    }

    endpoint_.close();

    anonymousMessageManager_.reset();
    peerCandidateManager_.reset();
    peerManager_.reset();

    packetCoder_->reset();

    p2pProperty_.reset();

    systemService_.reset();
}


void P2pSessionImpl::host(size_t maxPeers, bool hostMigration)
{
    assert(endpoint_.isOpened());
    assert(! isHost());

    p2pProperty_.maxPeers_ = static_cast<srpc::UInt8>(maxPeers);
    p2pProperty_.hostMigration_ = hostMigration;

    makeHost(myPeerId_);
}


void P2pSessionImpl::connect(const PeerAddresses& hostAddresses)
{
    assert(endpoint_.isOpened());
    assert(! isHost());

    connectToPeer(pseudoHostPeerId, toAddresses(hostAddresses));
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


void P2pSessionImpl::addMyPeer()
{
    assert(! endpoint_.getLocalAddresses().empty());
    peerManager_.addPeer(myPeerId_, endpoint_.getLocalAddresses());
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
            eventHandler_.onConnectFailed(peerId);
        }
    }
}


void P2pSessionImpl::resolve()
{
    if (! peerManager_.isExists(relayServerPeerId)) {
        return;
    }

    stunService_.resolve(getPeerTime());
}


void P2pSessionImpl::tryToConnect(PeerId peerId,
    const ACE_INET_Addr& targetAddress, const ACE_INET_Addr& peerAddress)
{
    assert(! peerManager_.isExists(peerId));

    peerCandidateManager_.addCandidate(peerId,
        AddressPair(targetAddress, peerAddress));
    const P2pPeerHint hint(peerId, &targetAddress, true);
    const PeerPtr me(peerManager_.getMe());
    systemService_.rpcConnect(me->getPeerAddresses(),
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
    targetAddress;
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


bool P2pSessionImpl::loopBack(PeerId peerId, ACE_Message_Block* mblock)
{
    if (isSelf(peerId) || isBroadcast(peerId)) {
        const char* rdPtr = mblock->rd_ptr();
        mblock->rd_ptr(packetCoder_->getHeaderSize());
        const PeerPtr me(peerManager_.getMe());
        (void)handleIncomingMessage(me->getPeerId(),
            Message(invalidSequenceNumber, mblock, me->getTargetAddress(), 0));
        mblock->rd_ptr(const_cast<char*>(rdPtr));
        if (! isBroadcast(peerId)) {
            return true;
        }
    }
    return false;
}


void P2pSessionImpl::disconnected(PeerId peerId)
{
    if (! peerManager_.isExists(peerId)) {
        return;
    }

    eventHandler_.onPeerDisconnected(peerId);

    if (peerManager_.isHost(peerId) || (! peerManager_.isHostExists())) {
        migrateHost();
    }
}


void P2pSessionImpl::migrateHost()
{
    if (p2pProperty_.hostMigration_ && peerManager_.isHostCandidate()) {
        systemService_.rpcHostMigrated();
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
        if (encodeMessage(*mblockGuard, peerIdPair, packetType, sequenceNumber,
            sentTime)) {
            if (! endpoint_.send(addressPair.targetAddress_, *mblockGuard)) {
                return false;
            }
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
    ACE_INET_Addr targetAddress;
    if (peerHint != 0) {
        peerId = peerHint->peerId_;
        targetAddress = peerHint->getAddress();
        if (peerHint->candidate_) {
            if (peerCandidateManager_.putOutgoingMessage(peerId, targetAddress,
                mblock)) {
                return;
            }
        }
    }

    if (peerManager_.isExists(peerId) || isBroadcast(peerId)) {
        if (loopBack(peerId, mblock)) {
            return;
        }
        
        if (! isSelf(peerId)) {
            peerManager_.putOutgoingMessage(peerId, targetAddress, packetType,
                mblock);
            return;
        }
    }

    //NSRPC_LOG_DEBUG3(
    //    ACE_TEXT("P2pSessionImpl::sendOutgoingMessage(P%u,%d)")
    //    ACE_TEXT("- Target missed"),
    //    peerId, packetType);
}


size_t P2pSessionImpl::getHeaderSize() const
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

    eventHandler_.onPeerConnected(peerId);

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
    const ACE_INET_Addr& targetAddress, const RAddresses& peerAddresses)
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
        peerManager_.addPeer(peerId, addresses);
        connected = true;
        firstConnection = true;
    }
    else if (! isRelayServerAddress(targetAddress)) {
        connected = true;
    }

    if (connected) {
        const PeerPtr peer(peerManager_.getPeer(peerId));
        peer->setTargetAddress(targetAddress);
        NSRPC_LOG_DEBUG5(ACE_TEXT("Peer(P%u) %sconnected via %s:%d."),
            peerId, (firstConnection ? "" : "re"),
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

    eventHandler_.onHostMigrated(peerId);
}


bool P2pSessionImpl::isHostConnected() const
{
    return (! isHost()) && peerManager_.isHostConnected();
}


void P2pSessionImpl::resolved(const srpc::String& ipAddress, srpc::UInt16 port)
{
    const PeerPtr me(peerManager_.getMe());
    if (me.isNull()) {
        return;
    }

    const ACE_INET_Addr resolvedAddress(port, ipAddress.c_str());
    me->setResolvedAddress(resolvedAddress);
    eventHandler_.onAddressResolved(ipAddress, port);
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

} // namespace detail

} // namespace nsrpc
