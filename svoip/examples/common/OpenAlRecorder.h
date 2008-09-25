#ifndef SVOIP_OPENAL_PLAYER_H
#define SVOIP_OPENAL_PLAYER_H

#include "svoip/Recorder.h"
#include "alc.h"
#include <boost/array.hpp>

/**
* @class OpenAlRecorder
* OpenAL Audio record & play
*/
class OpenAlRecorder : public svoip::Recorder
{
public:
    OpenAlRecorder(svoip::RecorderCallback* callback = 0);
    virtual ~OpenAlRecorder();

    virtual bool open();

    virtual void start();
    virtual void start(nsrpc::PeerId to);
    virtual void start(nsrpc::GroupId to);

    virtual void stop();

    virtual bool run();

private:
    void record();

    void resetTarget() {
        targetPeerId_ = nsrpc::invalidPeerId;
        targetGroupId_ = nsrpc::giUnknown;
    }

    size_t getAvailableSamples() const;

private:
    ALCdevice* captureDevice_;
    bool isRecording_;

    nsrpc::PeerId targetPeerId_;
    nsrpc::GroupId targetGroupId_;

    /// audio capture is always MONO16 (and that's what speex wants!).
    /// 2048 will cover 12 uncompressed frames in narrow-band mode.
    boost::array<svoip::Sample, svoip::sampleBufferSize> sampleBuffer_;
};

#endif // SVOIP_OPENAL_PLAYER_H
