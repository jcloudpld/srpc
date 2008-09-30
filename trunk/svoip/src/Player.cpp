#include "stdafx.h"
#include "svoip/Player.h"
#include "Task.h"
#include "Decoder.h"
#include "nsrpc/utility/AceUtil.h"
#include "nsrpc/utility/Logger.h"

namespace svoip
{

namespace detail
{

/**
 * @class PlayerTask
 */
class PlayerTask : public Task
{
public:
    PlayerTask(Player& player) :
        player_(player) {}

private:
    virtual int svc() {
        const ACE_Time_Value sleeptm = nsrpc::makeTimeValue(1);

        while (! shouldStop()) {
            if (! player_.run()) {
                NSRPC_LOG_ERROR("SVOIP: Player::run() FAILED!");
            }
            ACE_OS::sleep(sleeptm);
        }

        return 0;
    }

private:
    Player& player_;
};

} // namespace detail

// = Player

Player::Player(bool useThread) :
    useThread_(useThread)
{
}


Player::~Player()
{
}


bool Player::open()
{
    if (useThread_) {
        task_.reset(new detail::PlayerTask(*this));
        if (! task_->start()) {
            return false;
        }
    }

    return true;
}


void Player::close()
{
    stop();

    if (task_) {
        task_->stop();
    }
}


void Player::play(nsrpc::PeerId fromPeerId,
    const svoip::EncodedSample* sample, size_t samples, size_t frames,
    Speech speech, Sequence sequence)
{
    DecoderPtr decoder = getDecoder(fromPeerId);
    if (decoder.isNull()) {
        return;
    }

    size_t decodedSamples;
    svoip::Sample* decodedSample =
        decoder->decode(sample, samples, frames, speech, sequence, decodedSamples);

    playNow(decodedSample, decodedSamples);
}


void Player::addDecoder(nsrpc::PeerId peerId)
{
    const DecoderMap::const_iterator pos = decoderMap_.find(peerId);
    if (pos != decoderMap_.end()) {
        NSRPC_LOG_DEBUG2("SVOIP: Decoder(P#u) already exists.", peerId);
        return;
    }

    DecoderPtr decoder(new Decoder);
    if (! decoder->initialize()) {
        NSRPC_LOG_ERROR2("SVOIP: Failed to initialize Decoder(P#u).", peerId);
        return;
    }

    decoderMap_.insert(DecoderMap::value_type(peerId, decoder));
}


void Player::removeDecoder(nsrpc::PeerId peerId)
{
    const DecoderMap::iterator pos = decoderMap_.find(peerId);
    if (pos == decoderMap_.end()) {
        NSRPC_LOG_DEBUG2("SVOIP: Decoder(P#u) not found.", peerId);
        return;
    }

    decoderMap_.erase(pos);
}


Player::DecoderPtr Player::getDecoder(nsrpc::PeerId peerId)
{
    const DecoderMap::iterator pos = decoderMap_.find(peerId);
    if (pos == decoderMap_.end()) {
        return DecoderPtr();
    }

    return (*pos).second;
}

} // namespace svoip
