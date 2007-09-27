#include "stdafx.h"
#include <nsrpc/p2p/server/SimpleRelayService.h>
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/p2p/P2pEventHandler.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
#include <srpc/utility/Endian.h>
#include <ace/Reactor.h>
#include <cassert>

namespace nsrpc
{

namespace detail
{

IMPLEMENT_SRPC_EVENT_DISPATCHER(SimpleRelayService);

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

SimpleRelayService::SimpleRelayService(ACE_Reactor* reactor,
    PacketCoder* packetCoder, bool useBitPacking) :
    reactor_(reactor),
    packetCoder_(packetCoder),
    recvBlock_(new ACE_Message_Block(packetCoder_->getDefaultPacketSize())),
    sendBlock_(new ACE_Message_Block(packetCoder_->getDefaultPacketSize())),
    rpcNetwork_(*this, *recvBlock_, *sendBlock_, useBitPacking),
    endpoint_(*this, *recvBlock_, reactor_),
    outgoingUnreliableSequenceNumber_(invalidSequenceNumber),
    lastRelayPeerIdPair_(invalidPeerId, invalidPeerId),
    peerCipherKeys_(*packetCoder_)
{
    srpc::RpcReceiver::setRpcNetwork(&rpcNetwork_);
    srpc::RpcForwarder::setRpcNetwork(&rpcNetwork_);
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

SimpleRelayService::~SimpleRelayService()
{
    close();
    recvBlock_->release();
    sendBlock_->release();
}


bool SimpleRelayService::open(srpc::UInt16 port,
    const std::string& cipherKey)
{
    if (! endpoint_.open(port)) {
        return false;
    }

    if (! cipherKey.empty()) {
        peerCipherKeys_.set(relayServerPeerId, cipherKey);
    }
    packetCoder_->setDecryptSeed(peerCipherKeys_.get(relayServerPeerId));

    return true;
}


void SimpleRelayService::close()
{
    endpoint_.close();
}

// = overriding

void SimpleRelayService::onMessageArrived(const ACE_INET_Addr& peerAddress)
{
    P2pPacketHeader header;
    if (! packetCoder_->readHeader(header, *recvBlock_)) {
        NSRPC_LOG_ERROR3(
            ACE_TEXT("SimpleRelayService::onMessageArrived(from: %s:%d) ")
            ACE_TEXT("FAILED!!!(Invalid Message Header)"),
            peerAddress.get_host_addr(), peerAddress.get_port_number());
        return;
    }

    if (! packetCoder_->isValidHeader(header, *recvBlock_)) {
        NSRPC_LOG_ERROR3(
            ACE_TEXT("SimpleRelayService::onMessageArrived(from: %s:%d) ")
            ACE_TEXT("FAILED!!!(Message body is too short)"),
            peerAddress.get_host_addr(), peerAddress.get_port_number());
        return;
    }

    assert(! isReliable(header.packetType_));

    if (! packetCoder_->decode(*recvBlock_)) {
        return;
    }

    packetCoder_->advanceToBody(*recvBlock_);
    if (! rpcNetwork_.handleMessage(header.peerId_, peerAddress)) {
        NSRPC_LOG_ERROR3(
            ACE_TEXT("SimpleRelayService::onMessageArrived(from: %s:%d) ")
            ACE_TEXT("FAILED!!!(RpcNetwork::handleMessage() Error)"),
            peerAddress.get_host_addr(), peerAddress.get_port_number());
    }
}


bool SimpleRelayService::sendNow(const PeerIdPair& /*peerIdPair*/,
    const AddressPair& /*addressPair*/, const ACE_Message_Block& /*mblock*/,
    srpc::RpcPacketType /*packetType*/, SequenceNumber /*sequenceNumber*/,
    srpc::UInt32 /*sentTime*/)
{
    assert(false);
    return true;
}


void SimpleRelayService::marshalingErrorOccurred()
{
    endpoint_.close();
}


void SimpleRelayService::sendOutgoingMessage(srpc::RpcPacketType packetType,
    ACE_Message_Block* mblock, const P2pPeerHint* peerHint)
{
    assert(peerHint != 0);
    if (! peerHint) {
        return;
    }

    // TODO: P2pEndpoint::send()가 실패할 경우의 처리? 큐 & 재전송?

    P2pPacketHeader header(packetType, relayServerPeerId,
        ++outgoingUnreliableSequenceNumber_, 0);
    packetCoder_->setEncryptSeed(peerCipherKeys_.get(peerHint->peerId_));
    if (packetCoder_->encode(*mblock, header)) {
        (void)endpoint_.send(peerHint->getAddress(), *mblock);
    }
    else {
        NSRPC_LOG_ERROR(
            ACE_TEXT("SimpleRelayService - packet encoding FAILED!!!"));
    }
}


size_t SimpleRelayService::getHeaderSize() const
{
    return packetCoder_->getHeaderSize();
}

// = RpcStunService

IMPLEMENT_SRPC_P2P_METHOD_0(SimpleRelayService, rpcResolve, srpc::ptUnreliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());

    ACE_INET_Addr peerAddress(hint.getAddress());

    NSRPC_LOG_DEBUG4("RELAY|rpcResolve(P%u, %s:%d)",
        hint.peerId_, peerAddress.get_host_addr(),
        peerAddress.get_port_number());

    rpcResolved(peerAddress.get_host_addr(), peerAddress.get_port_number(),
        &hint);
}


DEFINE_SRPC_P2P_METHOD_2(SimpleRelayService, rpcResolved,
    srpc::RShortString, ipAddress, srpc::RUInt16, port,
    srpc::ptUnreliable);

// = RpcRelayService

IMPLEMENT_SRPC_P2P_METHOD_6(SimpleRelayService, rpcRelay,
    RPeerIdPair, peerIdPair, RAddress, peerAddress,
    RMessageBuffer, messageBlock, srpc::RRpcPacketType, packetType,
    RSequenceNumber, sequenceNumber, RRelativeTime, sentTime,
    srpc::ptUnreliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid()); hint;

    const ACE_INET_Addr fromAddress(hint.getAddress());
    char fromIp[INET_ADDRSTRLEN];
    fromAddress.get_host_addr(fromIp, INET_ADDRSTRLEN);
    char toIp[INET_ADDRSTRLEN];
    peerAddress.get_host_addr(toIp, INET_ADDRSTRLEN);
    NSRPC_LOG_DEBUG8("RELAY|rpcRelay(P%u->P%u, %s:%d->%s:%d, %d)",
        peerIdPair.from_, peerIdPair.to_,
        fromIp, fromAddress.get_port_number(),
        toIp, peerAddress.get_port_number(),
        messageBlock.bufferSize_);

    const P2pPeerHint toHint(peerIdPair.to_, &peerAddress);
    rpcRelayed(peerIdPair.from_, messageBlock, packetType, sequenceNumber,
        sentTime, &toHint);

    lastRelayPeerIdPair_ = peerIdPair;
}


DEFINE_SRPC_P2P_METHOD_5(SimpleRelayService, rpcRelayed,
    RPeerId, peerId, RMessageBuffer, messageBlock,
    srpc::RRpcPacketType, packetType, RSequenceNumber, sequenceNumber,
    RRelativeTime, sentTime, srpc::ptUnreliable);

} // namespace detail

} // namespace nsrpc
