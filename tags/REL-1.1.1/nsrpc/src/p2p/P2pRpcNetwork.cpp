#include "stdafx.h"
#include <nsrpc/p2p/detail/P2pRpcNetwork.h>
#include <nsrpc/p2p/detail/PeerNetworkSender.h>
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/detail/MessageBlockStreamBuffer.h>
#include <nsrpc/utility/Logger.h>
#include <srpc/detail/RpcCommand.h>
#include <srpc/StreamFactory.h>

namespace
{

inline srpc::StreamFactory::StreamType getStreamType(bool useBitPacking)
{
    return useBitPacking ?
        srpc::StreamFactory::stBit : srpc::StreamFactory::stByte;
}

} // namespace

namespace nsrpc
{

namespace detail
{

P2pRpcNetwork::P2pRpcNetwork(PeerNetworkSender& networkSender,
    ACE_Message_Block& recvBlock, ACE_Message_Block& sendBlock,
    bool useBitPacking) :
    networkSender_(networkSender),
    useBitPacking_(useBitPacking),
    rstreamBuffer_(new MessageBlockStreamBuffer(&recvBlock)),
    istream_(
        srpc::StreamFactory::createIStream(
            getStreamType(useBitPacking_), *rstreamBuffer_)),
    wstreamBuffer_(new MessageBlockStreamBuffer(&sendBlock)),
    ostream_(
        srpc::StreamFactory::createOStream(
            getStreamType(useBitPacking_), *wstreamBuffer_))
{
}


P2pRpcNetwork::~P2pRpcNetwork()
{
}


bool P2pRpcNetwork::handleMessage(PeerId peerId,
    const ACE_INET_Addr& peerAddr)
{
    const P2pPeerHint hint(peerId, &peerAddr);

    try {
        onReceive(*istream_, &hint);
    }
    catch (const srpc::Exception& e) {
        NSRPC_LOG_DEBUG3(
            ACE_TEXT("P2pRpcNetwork::handleMessage(P%u) FAILED(%s)."),
            peerId, e.what());
        return false;
    }

    istream_->reset(true);
    return true;
}


void P2pRpcNetwork::send(srpc::RpcCommand& command,
    srpc::RpcPacketType packetType, const void* rpcHint)
{
    ACE_Message_Block& wblock = initOutputStream();

    try {
        command.marshal(*ostream_);
    }
    catch (const srpc::Exception& e) {
        NSRPC_LOG_DEBUG2(ACE_TEXT("P2pRpcNetwork::send() - ")
            ACE_TEXT("RpcCommand::marshal() FAILED(%s)."),
            e.what());

        networkSender_.marshalingErrorOccurred();
        return;
    }

    networkSender_.sendOutgoingMessage(packetType, &wblock,
        static_cast<const P2pPeerHint*>(rpcHint));
}


ACE_Message_Block& P2pRpcNetwork::initOutputStream()
{
    ostream_->reset(true);
    ACE_Message_Block& wblock = *(wstreamBuffer_->getBlock());
    wblock.wr_ptr(networkSender_.getHeaderSize());
    return wblock;
}

} // namespace detail

} // namespace nsrpc
