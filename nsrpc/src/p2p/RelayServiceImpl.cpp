#include "stdafx.h"
#include "RelayServiceImpl.h"
#include "RelayServiceHandler.h"
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/utility/LogManager.h>
#include <nsrpc/utility/Logger.h>
#include <cassert>

namespace nsrpc
{

namespace detail
{

// = RpcRelayServiceImpl

IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcRelayServiceImpl);

RpcRelayServiceImpl::RpcRelayServiceImpl(RelayServiceHandler& serviceHandler,
    srpc::RpcNetwork& rpcNetwork) :
    srpc::RpcReceiver(&rpcNetwork),
    srpc::RpcForwarder(&rpcNetwork),
    serviceHandler_(serviceHandler)
{
}


FORWARD_SRPC_P2P_METHOD_6(RpcRelayServiceImpl, rpcRelay,
    RPeerIdPair, peerIdPair, RAddress, peerAddress,
    RMessageBuffer, messageBlock, srpc::RRpcPacketType, packetType,
    RSequenceNumber, sequenceNumber, RRelativeTime, sentTime,
    srpc::ptUnreliable);


EXCHANGE_SRPC_P2P_METHOD_5(RpcRelayServiceImpl, rpcRelayed,
    RPeerId, peerId, RMessageBuffer, messageBlock,
    srpc::RRpcPacketType, packetType, RSequenceNumber, sequenceNumber,
    RRelativeTime, sentTime, srpc::ptUnreliable)
{
    assert(rpcHint != 0);
    const P2pPeerHint& hint = *static_cast<const P2pPeerHint*>(rpcHint);
    assert(hint.isValid());

    ACE_Message_Block* mblock =
        serviceHandler_.acquire(messageBlock.getBufferLength());
    const void* vbuffer = messageBlock.getBuffer();
    mblock->copy(static_cast<const char*>(vbuffer),
        messageBlock.getBufferLength());
    serviceHandler_.relayed(peerId, hint.getAddress(), mblock, packetType,
        sequenceNumber, sentTime);

    if (LogManager::isEnabledMask(LM_DEBUG)) {
        const ACE_INET_Addr fromAddress(hint.getAddress());
        NSRPC_LOG_DEBUG2("rpcRelayed(%u bytes)",
            messageBlock.getBufferLength());
    }
}

} // namespace detail

} // namespace nsrpc
