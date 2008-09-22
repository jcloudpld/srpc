#include "stdafx.h"
#include "OpenAlPlayer.h"

OpenAlPlayer::OpenAlPlayer() :
    source_(0)
{

}


OpenAlPlayer::~OpenAlPlayer()
{
    if (source_ != 0) {
        alDeleteSources(1, &source_);
        source_ = 0;
    }
}


bool OpenAlPlayer::open()
{
    if (! Player::open()) {
        return false;
    }

    alGenSources(1, &source_);
    if (alGetError() != AL_NO_ERROR) {
        return false;
    }

    return true;
}


void OpenAlPlayer::start()
{
    assert(source_ != 0);

    alSourcePlay( source_ );
}


void OpenAlPlayer::stop()
{
    assert(source_ != 0);

    // Stop the Source and clear the Queue
    alSourceStop(source_);
    alSourcei(source_, AL_BUFFER, 0);
}


void OpenAlPlayer::play(nsrpc::PeerId fromPeerId,
    const svoip::EncodedSample* sample, size_t samples, size_t frames)
{
    assert(source_ != 0);

    size_t decodedSamples;
    svoip::Sample* decodedSample = decode(fromPeerId, sample, samples, frames,
        decodedSamples);

    play(decodedSample, decodedSamples);
}


bool OpenAlPlayer::run()
{
    assert(source_ != 0);

    // Un-queue any buffers, and delete them
    ALint processedBuffers;
    alGetSourcei(source_, AL_BUFFERS_PROCESSED, &processedBuffers);
    while (processedBuffers--) {
        ALuint buffer;
        alSourceUnqueueBuffers(source_, 1, &buffer);
        alDeleteBuffers(1, &buffer);
    }

    // Check the status of the Source.  If it is not playing,
    // then playback was completed, or the Source was starved of audio data,
    // and needs to be restarted.
    ALint state;
    alGetSourcei(source_, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
        // If there are Buffers in the Source Queue then the Source was
        // starved of audio data, so needs to be restarted
        // (because there is more audio data to play)
        ALint queuedBuffers;
        alGetSourcei(source_, AL_BUFFERS_QUEUED, &queuedBuffers);
        if (queuedBuffers) {
            alSourcePlay(source_);
        }
    }

    if (alGetError() != AL_NO_ERROR) {
        return false;
    }

    return true;
}


void OpenAlPlayer::play(const svoip::Sample* sample, size_t samples)
{
    assert(source_ != 0);

    ALuint alBuffer;
    alGenBuffers(1, &alBuffer);
    alBufferData(alBuffer, AL_FORMAT_MONO16,
        reinterpret_cast<ALvoid *>(const_cast<svoip::Sample*>(sample)),
        static_cast<ALsizei>(samples * sizeof(svoip::Sample)),
        svoip::frequency);

    // Shove the data onto the streamSource
    alSourceQueueBuffers(source_, 1, &alBuffer);
}
