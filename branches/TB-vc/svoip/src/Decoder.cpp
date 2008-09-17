#include "stdafx.h"
#include "Decoder.h"
#include "speex/speex.h"

namespace svoip
{

namespace detail
{

/**
 * @class DecoderImpl
 */
class DecoderImpl : public boost::noncopyable
{
public:
    DecoderImpl() :
        decoderState_(0),
        frameSize_(0) {}

    ~DecoderImpl() {
        if (decoderState_ != 0) {
            speex_decoder_destroy(decoderState_);
            speex_bits_destroy(&bits_);
            decoderState_ = 0;
        }
    }

    bool initialize() {
        speex_bits_init(&bits_);
        speex_bits_reset(&bits_);

        decoderState_ = speex_decoder_init(&speex_nb_mode); // narrow-band mode
        if (! decoderState_) {
            assert(false);
            return false;
        }

        if (0 != speex_decoder_ctl(decoderState_, SPEEX_GET_FRAME_SIZE,
            &frameSize_)) {
            assert(false);
            return false;
        }

        return true;
    }

    Sample* decode(const EncodedSample* sampleBuffer, size_t samples,
        size_t frames, size_t& decodedSamples) {
        assert(decoderState_ != 0);

        static Sample decodedSample[bufferSize];

        EncodedSample* currentSample = const_cast<EncodedSample*>(sampleBuffer);
        int written = 0;
        for (size_t i = 0; i < frames; ++i) {
            speex_bits_reset(&bits_);

            const int sampleLen = *currentSample;
            speex_bits_read_from(&bits_,
                reinterpret_cast<char*>(currentSample + 1), sampleLen);
            if (0 != speex_decode_int(decoderState_, &bits_,
                decodedSample + written)) {
                assert(false);
            }

            currentSample += (sampleLen + 1);
            written += frameSize_;
        }

        assert(samples == size_t(currentSample - sampleBuffer)); samples;

        decodedSamples = written;
        return decodedSample;
    }

private:
    SpeexBits bits_;
    void* decoderState_;
    int frameSize_;
};

} // namespace detail

Decoder::Decoder() :
    impl_(new detail::DecoderImpl)
{
}


Decoder::~Decoder()
{
}


bool Decoder::initialize()
{
    return impl_->initialize();
}


Sample* Decoder::decode(const EncodedSample* sampleBuffer, size_t samples,
    size_t frames, size_t& decodedSamples)
{
    return impl_->decode(sampleBuffer, samples, frames, decodedSamples);
}

} // namespace svoip
