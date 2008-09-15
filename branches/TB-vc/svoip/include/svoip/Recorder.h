#ifndef SVOIP_RECORDER_H
#define SVOIP_RECORDER_H

#include "config/config.h"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace svoip
{

class Encoder;

namespace detail
{
class RecorderTask;
} // namespace detail


/**
 * @class RecorderCallback
 */
class RecorderCallback
{
public:
    virtual ~RecorderCallback() {}

    virtual void sampled(EncodedSample* sample, size_t sampleLen,
        size_t frames) = 0;
};


/**
 * @class Recorder
 * Audio record & encode
 * 8kHz, 16bit, Mono
 */
class SVOIP_API Recorder : public boost::noncopyable
{
public:
    Recorder(RecorderCallback& callback);
    virtual ~Recorder();

    virtual bool open();

    virtual void close();

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual bool run() = 0;

protected:
    void encode(Sample* sampleBuffer, size_t samples);

    size_t getFrameSize() const;

private:
    RecorderCallback& callback_;
    boost::scoped_ptr<Encoder> encoder_;
    boost::scoped_ptr<detail::RecorderTask> task_;
};

} // namespace svoip

#endif // SVOIP_RECORDER_H
