#ifndef SVOIP_RECORDER_H
#define SVOIP_RECORDER_H

#include "config/config.h"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace svoip
{

class Encoder;
class RecorderCallback;

namespace detail
{
class RecorderTask;
} // namespace detail


/**
 * @class Recorder
 * Audio record & encode
 * 8kHz, 16bit, Mono
 */
class SVOIP_API Recorder : public boost::noncopyable
{
public:
    Recorder(RecorderCallback* callback = 0);
    virtual ~Recorder();

    virtual bool open();

    virtual void close();

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual bool run() = 0;

public:
    void setCallback(RecorderCallback* callback) {
        callback_ = callback;
    }

protected:
    void encode(Sample* sampleBuffer, size_t samples);

    size_t getFrameSize() const;

private:
    RecorderCallback* callback_;
    boost::scoped_ptr<Encoder> encoder_;
    boost::scoped_ptr<detail::RecorderTask> task_;
};

} // namespace svoip

#endif // SVOIP_RECORDER_H
