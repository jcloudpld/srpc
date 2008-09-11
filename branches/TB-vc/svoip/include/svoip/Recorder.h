#ifndef SVOIP_RECORDER_H
#define SVOIP_RECORDER_H

#include "config/config.h"
#include <boost/noncopyable.hpp>

namespace svoip
{

/**
 * @class Recorder
 * Audio record & encode
 */
class Recorder : public boost::noncopyable
{
public:
    virtual ~Recorder() {}

    virtual bool open() = 0;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual bool run() = 0;

    virtual Sample* getSample(size_t& samples) = 0;
};

} // namespace svoip

#endif // SVOIP_RECORDER_H
