#ifndef SVOIP_ENCODER_H
#define SVOIP_ENCODER_H

#include "svoip/config/config.h"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace svoip
{

namespace detail
{
class EncoderImpl;
} // namespace detail


/**
 * @class Encoder
 * Using Speex narrow-band mode
 */
class Encoder : public boost::noncopyable
{
public:
    Encoder();
    ~Encoder();

    bool initialize();

    EncodedSample* encode(Sample* sampleBuffer, size_t samples,
        size_t& encodedSamples, size_t& frames);

    size_t getFrameSize() const;

private:
    boost::scoped_ptr<detail::EncoderImpl> impl_;
};

} // namespace svoip

#endif // SVOIP_ENCODER_H
