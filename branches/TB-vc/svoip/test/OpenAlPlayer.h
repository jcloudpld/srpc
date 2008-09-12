#ifndef SVOIP_OPENAL_RECORDER_H
#define SVOIP_OPENAL_RECORDER_H

#include "svoip/Player.h"
#include "al.h"

/**
* @class OpenAlPlayer
* OpenAL Audio decode & play
*/
class OpenAlPlayer : public svoip::Player
{
public:
    OpenAlPlayer();
    virtual ~OpenAlPlayer();

    virtual bool open();

    virtual void start();

    virtual void stop();

    virtual void play(svoip::EncodedSample* sample, size_t samples,
        size_t frames);

    virtual bool run();

private:
    void play(svoip::Sample* sample, size_t samples);

private:
    ALuint source_;
};

#endif // SVOIP_OPENAL_RECORDER_H
