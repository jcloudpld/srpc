#ifndef SVOIP_DECODER_H
#define SVOIP_DECODER_H

#include "svoip/config/config.h"
#include <nsrpc/utility/SmartPtr.h>
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
 * Using Speex narrow-band mode
 */
class Decoder : public nsrpc::SharedObject
{
public:
    Decoder();
    ~Decoder();

    bool initialize();

    Sample* decode(const EncodedSample* sampleBuffer, size_t samples,
        size_t frames, Speech speech, Sequence sequence, size_t& decodedSamples);

private:
    boost::scoped_ptr<detail::DecoderImpl> impl_;
};

} // namespace svoip

#endif // SVOIP_DECODER_H
