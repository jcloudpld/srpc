#include "stdafx.h"
#include "Decoder.h"
#include <nsrpc/utility/Logger.h>
#include "speex/speex.h"

namespace svoip
{

namespace detail
{

Sample decodedSample[bufferSize];
const size_t decodedSampleSize = sizeof(decodedSample);

/**
 * @class DecoderImpl
 */
class DecoderImpl : public boost::noncopyable
{
public:
    DecoderImpl() :
        decoderState_(0),
        frameSize_(0),
        maxFrames_(0),
        incomingSpeech_(0),
        incomingSequence_(0) {}

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

        maxFrames_ = decodedSampleSize / (frameSize_ * sizeof(Sample));

        return true;
    }

    Sample* decode(const EncodedSample* sampleBuffer, size_t samples,
        size_t frames, Speech speech, Sequence sequence, size_t& decodedSamples) {
        assert(decoderState_ != 0);

        //NSRPC_LOG_DEBUG5("decode(%d samples, %d frames, %d speech, %d sequence)",
        //    samples, frames, speech, sequence);

        int written = correctMissingFrames(speech, sequence, frames);

        EncodedSample* currentSample = const_cast<EncodedSample*>(sampleBuffer);
        for (size_t i = 0; i < frames; ++i) {
            assert(((written + frameSize_) * sizeof(Sample)) <
                decodedSampleSize);

            const int sampleLen = *currentSample;
            speex_bits_read_from(&bits_,
                reinterpret_cast<char*>(currentSample + sampleSizeLen), sampleLen);
            if (0 != speex_decode_int(decoderState_, &bits_,
                decodedSample + written)) {
                assert(false);
            }

            currentSample += (sampleLen + sampleSizeLen);
            written += frameSize_;
        }

        assert(samples == size_t(currentSample - sampleBuffer)); samples;

        incomingSequence_ = sequence + frames;

        decodedSamples = written;
        return decodedSample;
    }

private:
    int correctMissingFrames(Speech speech, Sequence sequence, size_t frames) {
        const int seqDiff = sequence - incomingSequence_;

        if (speech != incomingSpeech_) {
            // This is a new "speech" ... a new recording started.
            incomingSpeech_ = speech;
        }
        else if (seqDiff < 0) {   // we're ahead of the sequence?!
            // the packet is corrupted or something.
            NSRPC_LOG_DEBUG3("SVOIP: misordered sequence(%d < %d)",
                sequence, incomingSequence_);
        }
        else if (seqDiff > 100) { // more than 2 seconds of audio dropped?
            // just start over.
            NSRPC_LOG_DEBUG2("SVOIP: Dropped way too many frames(%d)", seqDiff);
        }
        else {
            if (seqDiff <= 0) {
                return 0;
            }

            const int spareFrames = static_cast<int>(maxFrames_ - frames);
            const int droppedFrames = (std::min)(seqDiff, spareFrames);

            NSRPC_LOG_DEBUG3("VoIP: Dropped %d frames, but %d skip",
                seqDiff, droppedFrames);
            // tell speex that we're missing frames...
            int written = 0;
            for (int i = 0; i < droppedFrames; ++i) {
                assert(((written + frameSize_) * sizeof(Sample)) <
                    decodedSampleSize);
                (void)speex_decode_int(decoderState_, NULL, decodedSample + written);
                written += frameSize_;
            }
            return written;
        }

        speex_bits_reset(&bits_);
        return 0;
    }

private:
    SpeexBits bits_;
    void* decoderState_;
    int frameSize_;
    size_t maxFrames_;
    Speech incomingSpeech_;
    Sequence incomingSequence_;
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
    size_t frames, Speech speech, Sequence sequence, size_t& decodedSamples)
{
    return impl_->decode(sampleBuffer, samples, frames, speech, sequence,
        decodedSamples);
}

} // namespace svoip
