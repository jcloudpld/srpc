#include "stdafx.h"
#include "svoip/Player.h"
#include "Decoder.h"
#include "nsrpc/utility/AceUtil.h"
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
 * @class PlayerTask
 */
class PlayerTask : public ACE_Task_Base
{
public:
    PlayerTask(Player& player) :
        player_(player),
        shouldStop_(false) {}

    virtual ~PlayerTask() {
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
            player_.run();
            ACE_OS::sleep(sleeptm);
        }

        return 0;
    }

private:
    Player& player_;
    volatile bool shouldStop_;
};

} // namespace detail

// = Player

Player::Player()
{
}


Player::~Player()
{
}


bool Player::open()
{
    decoder_.reset(new Decoder);
    if (! decoder_->initialize()) {
        return false;
    }

    task_.reset(new detail::PlayerTask(*this));
    if (! task_->start()) {
        return false;
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


svoip::Sample* Player::decode(svoip::EncodedSample* sample, size_t samples,
    size_t frames, size_t& decodedSamples)
{
    return decoder_->decode(sample, samples, frames, decodedSamples);
}

} // namespace svoip
