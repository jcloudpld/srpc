#include "stdafx.h"
#include "VoP2pPlugInImpl.h"
#include "svoip/Recorder.h"
#include "svoip/Player.h"
#include <nsrpc/p2p/P2pSession.h>
#include <nsrpc/p2p/detail/P2pPeerHint.h>
#include <nsrpc/utility/Logger.h>
#include <ace/Guard_T.h>

namespace svoip
{

VoP2pPlugInImpl::VoP2pPlugInImpl(std::auto_ptr<Recorder> recorder,
    std::auto_ptr<Player> player) :
    recorder_(recorder.release()),
    player_(player.release())
{
    recorder_->setCallback(this);
}


VoP2pPlugInImpl::~VoP2pPlugInImpl()
{
}


bool VoP2pPlugInImpl::initialize()
{
    if (! recorder_->open()) {
        NSRPC_LOG_ERROR("SVOIP: Recorder::create() Failed");
        return false;
    }

    if (! player_->open()) {
        NSRPC_LOG_ERROR("SVOIP: Player::create() Failed");
        return false;
    }

    player_->start();

    return true;
}


void VoP2pPlugInImpl::update()
{
    ACE_GUARD(ACE_Thread_Mutex, monitor, lock_);

    if (sampleQueue_.empty()) {
        return;
    }

    SampleChunk& chunk = sampleQueue_.front();
    say(chunk.buffer_, static_cast<srpc::UInt8>(chunk.frames_),
        chunk.speech_, chunk.sequence_, &chunk.hint_);
    sampleQueue_.pop();
}


void VoP2pPlugInImpl::attached(nsrpc::P2pSession* session)
{
    VoP2pPlugIn::attached(session);

    session->addP2pOptions(nsrpc::allowVoP2p);
}


void VoP2pPlugInImpl::onPeerConnected(nsrpc::PeerId peerId)
{
    player_->addDecoder(peerId);
}


void VoP2pPlugInImpl::onPeerDisconnected(nsrpc::PeerId peerId)
{
    player_->removeDecoder(peerId);
}


void VoP2pPlugInImpl::record()
{
    assert(recorder_);
    recorder_->start();
}


void VoP2pPlugInImpl::record(nsrpc::PeerId to)
{
    assert(recorder_);
    recorder_->start(to);
}


void VoP2pPlugInImpl::record(nsrpc::GroupId to)
{
    assert(recorder_);
    recorder_->start(to);
}


void VoP2pPlugInImpl::stop()
{
    assert(recorder_);

    recorder_->stop();
}


// = svoip::RecorderCallback overriding

void VoP2pPlugInImpl::sampled(nsrpc::PeerId targetPeerId,
    nsrpc::GroupId targetGroupId, EncodedSample* sample, size_t sampleLen,
    size_t frames, Speech speech, Sequence sequence)
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
    hint.p2pOptions_ = nsrpc::allowVoP2p;

    assert(frames < 256);

    sampleQueue_.push(
        SampleChunk(hint, sample, sampleLen, frames, speech, sequence));
}

// = RpcVoP2pService overriding

IMPLEMENT_SRPC_P2P_METHOD_4(VoP2pPlugInImpl, say,
    nsrpc::detail::RMessageBuffer, samples, srpc::RUInt8, frames,
    RSpeech, speech, RSequence, sequence,
    srpc::ptUnreliable)
{
    assert(player_);
    const nsrpc::PeerHint& hint = nsrpc::toPeerHint(rpcHint);

    player_->play(hint.peerId_,
        static_cast<const svoip::EncodedSample*>(samples.getBuffer()),
        samples.getBufferLength(), frames, speech, sequence);
}

} // namespace svoip
