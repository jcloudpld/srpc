#ifndef SVOIP_RECORDER_H
#define SVOIP_RECORDER_H

#include "config/config.h"
#include <nsrpc/p2p/PeerId.h>
#include <nsrpc/p2p/Group.h>
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
    friend class detail::RecorderTask;
public:
    Recorder(RecorderCallback* callback = 0);
    virtual ~Recorder();

    virtual bool open();

    virtual void close();

    /// broadcast
    virtual void start() = 0;

    /// unicast
    virtual void start(nsrpc::PeerId to) = 0;

    /// multicast
    virtual void start(nsrpc::GroupId to) = 0;

    virtual void stop() = 0;

private:
    virtual bool run() = 0;

public:
    void setCallback(RecorderCallback* callback) {
        callback_ = callback;
    }

protected:
    void setNewSpeech();

    void encode(nsrpc::PeerId targetPeerId, nsrpc::GroupId targetGroupId,
        Sample* sampleBuffer, size_t samples);

    size_t getFrameSize() const;

private:
    RecorderCallback* callback_;
    boost::scoped_ptr<Encoder> encoder_;
    boost::scoped_ptr<detail::RecorderTask> task_;

    Speech speech_;
    Sequence sequence_;
};

} // namespace svoip

#endif // SVOIP_RECORDER_H
