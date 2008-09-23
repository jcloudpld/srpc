#include "stdafx.h"
#include "svoip/Recorder.h"
#include "svoip/RecorderCallback.h"
#include "Encoder.h"
#include "nsrpc/utility/AceUtil.h"
#include "nsrpc/utility/Logger.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4355 4800)
#endif
#include <ace/Task.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace svoip
{

namespace detail
{

/**
 * @class RecorderTask
 */
class RecorderTask : public ACE_Task_Base
{
public:
    RecorderTask(Recorder& recorder) :
        recorder_(recorder),
        shouldStop_(false) {}

    virtual ~RecorderTask() {
        stop();
    }

    bool start() {
        return activate() == 0;
    }

    void stop() {
        shouldStop_ = true;
        wait();
    }

private:
    virtual int svc() {
        const ACE_Time_Value sleeptm = nsrpc::makeTimeValue(1);

        while (! shouldStop_) {
            if (! recorder_.run()) {
                NSRPC_LOG_ERROR("Recoder::run() FAILED!");
            }
            ACE_OS::sleep(sleeptm);
        }

        return 0;
    }

private:
    Recorder& recorder_;
    volatile bool shouldStop_;
};

} // namespace detail

// = Recorder

Recorder::Recorder(RecorderCallback* callback) :
    callback_(callback),
    speech_(0),
    sequence_(0)
{
}


Recorder::~Recorder()
{
}


bool Recorder::open()
{
    encoder_.reset(new Encoder);
    if (! encoder_->initialize()) {
        return false;
    }

    task_.reset(new detail::RecorderTask(*this));
    if (! task_->start()) {
        return false;
    }

    return true;
}


void Recorder::close()
{
    stop();

    if (task_) {
        task_->stop();
    }
}


void Recorder::setNewSpeech()
{
    ++speech_;
    if (speech_ <= 0) {
        speech_ = 1;
    }
    sequence_ = 0;
}


void Recorder::encode(nsrpc::PeerId targetPeerId, nsrpc::GroupId targetGroupId,
    Sample* sampleBuffer, size_t samples)
{
    assert(callback_ != 0);

    size_t encodedSamples;
    size_t frames;
    svoip::EncodedSample* encodedBuffer =
        encoder_->encode(sampleBuffer, samples, encodedSamples, frames);

    if (frames <= 0) {
        return;
    }

    callback_->sampled(targetPeerId, targetGroupId,
        encodedBuffer, encodedSamples, frames, speech_, sequence_);

    sequence_ += static_cast<Sequence>(frames);
}


size_t Recorder::getFrameSize() const
{
    return encoder_->getFrameSize();
}

} // namespace svoip
