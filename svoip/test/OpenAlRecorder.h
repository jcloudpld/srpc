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
    OpenAlRecorder();
    virtual ~OpenAlRecorder();

    virtual bool open();

    virtual void start();

    virtual void stop();

    virtual bool run();

    virtual svoip::Sample* getSample(size_t& samples);

private:
    size_t getAvailableCaptureSamples() const;

private:
    ALCdevice* captureDevice_;
};

#endif // SVOIP_OPENAL_PLAYER_H
