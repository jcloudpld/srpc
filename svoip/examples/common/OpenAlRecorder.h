#ifndef SVOIP_OPENAL_PLAYER_H
#define SVOIP_OPENAL_PLAYER_H

#include "svoip/Recorder.h"
#include "alc.h"

/**
* @class OpenAlRecorder
* OpenAL Audio record & play
*/
class OpenAlRecorder : public svoip::Recorder
{
public:
    OpenAlRecorder(svoip::RecorderCallback* callback);
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
};

#endif // SVOIP_OPENAL_PLAYER_H
