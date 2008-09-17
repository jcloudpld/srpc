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

    virtual void stop();

    virtual bool run();

private:
    size_t getAvailableSamples() const;

private:
    ALCdevice* captureDevice_;
    bool isRecording_;
};

#endif // SVOIP_OPENAL_PLAYER_H
