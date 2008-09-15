#ifndef SVOIP_PLAYER_H
#define SVOIP_PLAYER_H

#include "config/config.h"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace svoip
{

class Decoder;

namespace detail
{
class PlayerTask;
} // namespace detail


/**
 * @class Player
 * Audio decode & play
 */
class SVOIP_API Player : public boost::noncopyable
{
public:
    Player();
    virtual ~Player();

    virtual bool open();

    virtual void close();

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual void play(EncodedSample* sample, size_t samples,
        size_t frames) = 0;

    virtual bool run() = 0;

protected:
    svoip::Sample* decode(svoip::EncodedSample* sample, size_t samples,
        size_t frames, size_t& decodedSamples);

private:
    boost::scoped_ptr<Decoder> decoder_;
    boost::scoped_ptr<detail::PlayerTask> task_;
};

} // namespace svoip

#endif // SVOIP_PLAYER_H
