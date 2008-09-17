#ifndef SVOIP_DECODER_H
#define SVOIP_DECODER_H

#include "svoip/config/config.h"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace svoip
{

namespace detail
{
class DecoderImpl;
} // namespace detail


/**
 * @class Decoder
 */
class Decoder : public boost::noncopyable
{
public:
    Decoder();
    ~Decoder();

    bool initialize();

    Sample* decode(const EncodedSample* sampleBuffer, size_t samples,
        size_t frames, size_t& decodedSamples);

private:
    boost::scoped_ptr<detail::DecoderImpl> impl_;
};

} // namespace svoip

#endif // SVOIP_DECODER_H
