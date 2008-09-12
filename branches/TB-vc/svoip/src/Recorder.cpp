#include "stdafx.h"
#include "svoip/Recorder.h"
#include "Encoder.h"

namespace svoip
{

Recorder::Recorder(RecorderCallback& callback) :
    callback_(callback),
    encoder_(new Encoder)
{
}


Recorder::~Recorder()
{
}


bool Recorder::open()
{
    return encoder_->initialize();
}


void Recorder::encode(Sample* sampleBuffer, size_t samples)
{
    size_t encodedSamples;
    size_t frames;
    svoip::EncodedSample* encodedBuffer =
        encoder_->encode(sampleBuffer, samples, encodedSamples, frames);

    callback_.sampled(encodedBuffer, encodedSamples, frames);
}


size_t Recorder::getFrameSize() const
{
    return encoder_->getFrameSize();
}

} // namespace svoip
