#include "stdafx.h"
#include "AnonymousMessageManager.h"
#include "PeerMessageHandler.h"

namespace nsrpc
{

namespace detail
{

void AnonymousMessageManager::reset()
{
    incomingUnknownReliableMessages_.release();
    incomingUnknownUnreliableMessages_.release();
}


void AnonymousMessageManager::putIncomingMessage(const P2pPacketHeader& header,
    const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock)
{
    if (srpc::isReliable(header.packetType_)) {
        incomingUnknownReliableMessages_.insert(
            UnknownReliableMessage(header.sequenceNumber_, mblock,
                peerAddress, header.sentTime_, header.peerId_));
    }
    else {
        incomingUnknownUnreliableMessages_.insert(
            UnknownUnreliableMessage(header.sequenceNumber_, mblock,
                peerAddress, header.sentTime_, header.peerId_));
    }
}


void AnonymousMessageManager::handleIncomingMessages()
{
    handleIncomingReliableMessages();
    handleIncomingUnreliableMessages();
}


void AnonymousMessageManager::handleIncomingReliableMessages()
{
    UnknownReliableMessages::iterator pos =
        incomingUnknownReliableMessages_.begin();
    const UnknownReliableMessages::iterator end =
        incomingUnknownReliableMessages_.end();
    for (; pos != end; ++pos) {
        handleIncomingMessage((*pos).peerId_, *pos, srpc::ptReliable);
    }
    incomingUnknownReliableMessages_.clear();
}


void AnonymousMessageManager::handleIncomingUnreliableMessages()
{
    UnknownUnreliableMessages::iterator pos =
        incomingUnknownUnreliableMessages_.begin();
    const UnknownUnreliableMessages::iterator end =
        incomingUnknownUnreliableMessages_.end();
    for (; pos != end; ++pos) {
        handleIncomingMessage((*pos).peerId_, *pos, srpc::ptUnreliable);
    }
    incomingUnknownUnreliableMessages_.clear();
}


void AnonymousMessageManager::handleIncomingMessage(PeerId peerId,
    Message& message, srpc::RpcPacketType packetType)
{
    if (messageHandler_.handleIncomingMessage(peerId, message)) {
        if (isReliable(packetType)) {
            messageHandler_.sendAcknowledgement(peerId, message);
        }
    }

    message.release();
}

} // namespace detail

} // namespace nsrpc
