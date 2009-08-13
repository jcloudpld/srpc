#ifndef SVOIP_OPENAL_RECORDER_H
#define SVOIP_OPENAL_RECORDER_H

#ifdef _MSC_VER
#  pragma once
#endif

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

private:
    virtual void playNow(const svoip::Sample* sample, size_t samples);

    virtual bool run();

private:
    ALuint source_;
};

#endif // SVOIP_OPENAL_RECORDER_H
