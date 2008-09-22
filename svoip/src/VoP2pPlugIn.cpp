#include "stdafx.h"
#include "svoip/VoP2pPlugIn.h"
#include "svoip/Recorder.h"
#include "svoip/Player.h"
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/utility/Logger.h>
#include <ace/Guard_T.h>

namespace svoip
{

VoP2pPlugIn::VoP2pPlugIn()
{
}


VoP2pPlugIn::~VoP2pPlugIn()
{
}


bool VoP2pPlugIn::initialize()
{
    recorder_.reset(createRecorder().release());
    if (! recorder_->open()) {
        NSRPC_LOG_ERROR("VoP2pPlugIn::createRecorder() Failed");
        return false;
    }

    player_.reset(createPlayer().release());
    if (! player_->open()) {
        NSRPC_LOG_ERROR("VoP2pPlugIn::createPlayer() Failed");
        return false;
    }

    player_->start();

    return true;
}


void VoP2pPlugIn::update()
{
    ACE_GUARD(ACE_Thread_Mutex, monitor, lock_);

    if (sampleQueue_.empty()) {
        return;
    }

    SampleChunk& chunk = sampleQueue_.front();
    say(chunk.buffer_, static_cast<srpc::UInt8>(chunk.frames_), &chunk.hint_);
    sampleQueue_.pop();
}


void VoP2pPlugIn::onPeerConnected(nsrpc::PeerId peerId)
{
    player_->addDecoder(peerId);
}


void VoP2pPlugIn::onPeerDisconnected(nsrpc::PeerId peerId)
{
    player_->removeDecoder(peerId);
}


void VoP2pPlugIn::record()
{
    assert(recorder_);
    recorder_->start();
}


void VoP2pPlugIn::record(nsrpc::PeerId to)
{
    assert(recorder_);
    recorder_->start(to);
}


void VoP2pPlugIn::record(nsrpc::GroupId to)
{
    assert(recorder_);
    recorder_->start(to);
}


void VoP2pPlugIn::stop()
{
    assert(recorder_);

    recorder_->stop();
}


// = svoip::RecorderCallback overriding

void VoP2pPlugIn::sampled(nsrpc::PeerId targetPeerId,
    nsrpc::GroupId targetGroupId, EncodedSample* sample, size_t sampleLen,
    size_t frames)
{
    ACE_GUARD(ACE_Thread_Mutex, monitor, lock_);

    assert(recorder_);

    nsrpc::PeerHint hint(nsrpc::invalidPeerId);
    if (nsrpc::isValid(targetGroupId)) {
        hint.groupId_ = targetGroupId;
    }
    else if (nsrpc::isValidPeerId(targetPeerId)) {
        hint.peerId_ = targetPeerId;
    }

    assert(frames < 256);

    sampleQueue_.push(SampleChunk(hint, sample, sampleLen, frames));
}

// = RpcVoP2pService overriding

IMPLEMENT_SRPC_P2P_METHOD_2(VoP2pPlugIn, say,
    nsrpc::detail::RMessageBuffer, samples, srpc::RUInt8, frames,
    srpc::ptUnreliable)
{
    assert(player_);
    const nsrpc::PeerHint& hint = nsrpc::toPeerHint(rpcHint);

    player_->play(hint.peerId_,
        static_cast<const svoip::EncodedSample*>(samples.getBuffer()),
        samples.getBufferLength(), frames);
}

} // namespace svoip
