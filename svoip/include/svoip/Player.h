#ifndef SVOIP_PLAYER_H
#define SVOIP_PLAYER_H

#include "config/config.h"
#include <boost/noncopyable.hpp>

namespace svoip
{

/**
 * @class Player
 * Audio decode & play
 */
class Player : public boost::noncopyable
{
public:
    virtual ~Player() {}

    virtual bool open() = 0;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void play(Sample* sample, size_t samples) = 0;

    virtual bool run() = 0;
};

} // namespace svoip

#endif // SVOIP_PLAYER_H
