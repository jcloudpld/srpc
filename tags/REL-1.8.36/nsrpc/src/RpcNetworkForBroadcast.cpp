#include "stdafx.h"
#include <nsrpc/detail/RpcNetworkForBroadcast.h>
#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/detail/MessageBlockStreamBuffer.h>
#include <nsrpc/detail/CsProtocol.h>
#include <nsrpc/utility/MessageBlockManager.h>
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

RpcNetworkForBroadcast::RpcNetworkForBroadcast() :
    useBitPacking_(false),
    sendBlock_(0)
{
    srpc::RpcForwarder::setRpcNetwork(*this);
}


RpcNetworkForBroadcast::~RpcNetworkForBroadcast()
{
    releaseSendBlock();
}


void RpcNetworkForBroadcast::initialize(bool useBitPacking,
    PacketCoderFactory& packetCoderFactory)
{
    useBitPacking_ = useBitPacking;

    packetCoder_.reset(packetCoderFactory.create());
    messageBlockManager_.reset(
        new SynchMessageBlockManager(
            packetCoder_->getDefaultPacketPoolSize(),
            packetCoder_->getDefaultPacketSize()));
}


void RpcNetworkForBroadcast::prepareBroadcast()
{
    releaseSendBlock();
}


void* RpcNetworkForBroadcast::acquireBlock()
{
    return sendBlock_;
}


void RpcNetworkForBroadcast::send(srpc::RpcCommand& command,
    srpc::RpcPacketType /*packetType*/, const void* /*rpcHint*/)
{
    sendBlock_ = marshal(command);
}


void RpcNetworkForBroadcast::releaseSendBlock()
{
    if (sendBlock_ != 0) {
        sendBlock_->release();
        sendBlock_ = 0;
    }
}


ACE_Message_Block* RpcNetworkForBroadcast::marshal(
    srpc::RpcCommand& command)
{
    ACE_Message_Block* wblock = initOutputStream();

    try {
        command.marshal(*ostream_);
    }
    catch (const srpc::Exception& e) {
        NSRPC_LOG_DEBUG2(
            ACE_TEXT("RpcNetworkForBroadcast::marshal() - ")
            ACE_TEXT("RpcCommand::marshal() FAILED(%s)."),
            e.what());

        wblock->release();
        return 0;
    }

    return wblock;
}


ACE_Message_Block* RpcNetworkForBroadcast::initOutputStream()
{
    ACE_Message_Block* wblock = &acquireSendBlock();
    if (! ostream_.get()) { // lazy evaluation
        wstreamBuffer_.reset(new MessageBlockStreamBuffer(wblock));
        ostream_.reset(
            srpc::StreamFactory::createOStream(
                getStreamType(useBitPacking_), *wstreamBuffer_));
    }
    else {
        ostream_->reset(false);
        wstreamBuffer_->reset(wblock);
    }
    return wblock;
}

// = MessageBlockProvider overriding

ACE_Message_Block& RpcNetworkForBroadcast::acquireSendBlock()
{
    ACE_Message_Block* mblock =
        messageBlockManager_->create(packetCoder_->getDefaultPacketSize());
    packetCoder_->reserveHeader(*mblock);
    return *mblock;
}


ACE_Message_Block& RpcNetworkForBroadcast::acquireRecvBlock()
{
    assert(false && "DON'T CALL ME!");

    ACE_Message_Block* nullBlock = 0;
    return *nullBlock;
}

} // namespace nsrpc
