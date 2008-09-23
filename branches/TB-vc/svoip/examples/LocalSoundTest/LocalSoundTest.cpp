#include "stdafx.h"
#include "OpenAlRecorder.h"
#include "OpenAlPlayer.h"
#include "svoip/RecorderCallback.h"
#include "openal_framework/Framework.h"
#include "nsrpc/utility/AceUtil.h"
#include <boost/scoped_ptr.hpp>

namespace
{

/**
 * @class LocalPlayer
 */
class LocalPlayer : public OpenAlPlayer,
    public svoip::RecorderCallback
{
    static const nsrpc::PeerId defaultPeerId = nsrpc::invalidPeerId + 1;
public:
    virtual bool open() {
        if (! OpenAlPlayer::open()) {
            return false;
        }

        addDecoder(defaultPeerId);
        return true;
    }

private:
    virtual void sampled(nsrpc::PeerId /*targetPeerId*/,
        nsrpc::GroupId /*targetGroupId*/,
        svoip::EncodedSample* sample, size_t sampleLen, size_t frames,
        svoip::Speech speech, svoip::Sequence sequence) {
        play(defaultPeerId, sample, sampleLen, frames, speech, sequence);
    }
};


bool initializeOpenAl()
{
    ALFWInit();

    if (! ALFWInitOpenAL()) {
        ALFWprintf("Failed to initialize OpenAL\n");
        ALFWShutdown();
        return false;
    }

    return true;
}


void shutdownOpenAl()
{
    ALFWShutdownOpenAL();
}


bool run()
{
    nsrpc::InitAce ace;

    boost::scoped_ptr<LocalPlayer> player(new LocalPlayer);
    if (! player->open()) {
        return false;
    }

    boost::scoped_ptr<svoip::Recorder> recorder(
        new OpenAlRecorder(player.get()));
    if (! recorder->open()) {
        return false;
    }

    recorder->start();
    player->start();

    ALFWprintf("Press any key to finish.\n");
    while (! ALFWKeyPress()) {
        ::Sleep(1);
    }

    recorder->close();
    player->close();

    return true;
}

} // namespace

int main(int /*argc*/, char* /*argv*/[])
{
    ALFWprintf("LocalSoundTest with OpenAL & Speex.\n");

    if (! initializeOpenAl()) {
        return EXIT_FAILURE;
    }

    const bool result = run();

    shutdownOpenAl();

    return result ? EXIT_SUCCESS : EXIT_FAILURE;
}

