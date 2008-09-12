#include "stdafx.h"
#include "svoip/Player.h"
#include "Decoder.h"

namespace svoip
{

Player::Player() :
    decoder_(new Decoder)
{
}


Player::~Player()
{
}


bool Player::open()
{
    return decoder_->initialize();
}


svoip::Sample* Player::decode(svoip::EncodedSample* sample, size_t samples,
    size_t frames, size_t& decodedSamples)
{
    return decoder_->decode(sample, samples, frames, decodedSamples);
}

} // namespace svoip
