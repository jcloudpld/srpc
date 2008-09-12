#include "stdafx.h"
#include "Encoder.h"
#include "speex/speex.h"
#include "speex/speex_preprocess.h"

namespace svoip
{

namespace detail
{

/**
 * @class EncoderImpl
 */
class EncoderImpl : public boost::noncopyable
{
public:
    EncoderImpl() :
        encoderState_(0),
        frameSize_(0),
        sampleRate_(0),
        preprocessor_(0) {}

    ~EncoderImpl() {
        if (preprocessor_ != 0) {
            speex_preprocess_state_destroy(preprocessor_);
            preprocessor_ = 0;
        }

        if (encoderState_ != 0) {
            speex_encoder_destroy(encoderState_);
            speex_bits_destroy(&bits_);
            encoderState_ = 0;
        }
    }


    bool initialize() {
        speex_bits_init(&bits_);
        speex_bits_reset(&bits_);

        encoderState_ = speex_encoder_init(&speex_nb_mode); // narrow-band mode
        if (! encoderState_) {
            assert(false);
            return false;
        }

        if (0 != speex_encoder_ctl(encoderState_, SPEEX_GET_FRAME_SIZE,
            &frameSize_)) { // 160
            assert(false);
            return false;
        }

        if (0 != speex_encoder_ctl(encoderState_, SPEEX_GET_SAMPLING_RATE,
            &sampleRate_)) { // 8000
            assert(false);
            return false;
        }

        preprocessor_ = speex_preprocess_state_init(frameSize_, sampleRate_);
        if (! preprocessor_) {
            assert(false);
            return false;
        }

        int useDenoise = 1;
        (void)speex_preprocess_ctl(preprocessor_, SPEEX_PREPROCESS_SET_DENOISE,
            &useDenoise);

        int useAgc = 1;
        (void)speex_preprocess_ctl(preprocessor_, SPEEX_PREPROCESS_SET_AGC,
            &useAgc);

        return true;
    }

    EncodedSample* encode(Sample* sampleBuffer, size_t samples,
        size_t& encodedSamples, size_t& frames) {
        static EncodedSample encodedSample[sampleBufferSize / 2];

        frames = 0;
        int wpos = 0;
        for (size_t pos = 0; pos < samples; pos += frameSize_) {
            Sample* currentSample = &sampleBuffer[pos];

            denoise(currentSample);

            // encode raw audio samples into Speex data
            speex_bits_reset(&bits_);
            speex_encode_int(encoderState_, currentSample, &bits_);
            const int bytes = speex_bits_write(&bits_,
                reinterpret_cast<char *>(&encodedSample[wpos+1]),
                sizeof(encodedSample) - (wpos+1));
            assert((bytes > 0) && (bytes < 256));
            encodedSample[wpos] = static_cast<EncodedSample>(bytes);
            wpos += (bytes + 1);

            // look at the data for the next sample
            ++frames;
        }

        encodedSamples = wpos;
        return encodedSample;
    }

    size_t getFrameSize() const {
        return frameSize_;
    }

private:
    void denoise(Sample* sample) {
        // remove noise
        speex_preprocess_run(preprocessor_, sample);

        // check the "power" of this sample
        for (int i = 0; i < frameSize_; i++) {
            sample[i] = static_cast<Sample>(sample[i] * audioMult);
        }
    }

private:
    SpeexBits bits_;
    void* encoderState_;
    int frameSize_;
    int sampleRate_;

    SpeexPreprocessState* preprocessor_;
};

} // namespace detail

Encoder::Encoder() :
    impl_(new detail::EncoderImpl)
{
}


Encoder::~Encoder()
{
}


bool Encoder::initialize()
{
    return impl_->initialize();
}


EncodedSample* Encoder::encode(Sample* sampleBuffer, size_t samples,
    size_t& encodedSamples, size_t& frames)
{
    return impl_->encode(sampleBuffer, samples, encodedSamples, frames);
}


size_t Encoder::getFrameSize() const
{
    return impl_->getFrameSize();
}

} // namespace svoip
