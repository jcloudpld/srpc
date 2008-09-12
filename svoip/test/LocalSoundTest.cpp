#include "stdafx.h"
#include "OpenAlRecorder.h"
#include "OpenAlPlayer.h"
#include "openal_framework/Framework.h"
#include <boost/scoped_ptr.hpp>

namespace
{

/**
 * @class LocalPlayer
 */
class LocalPlayer : public OpenAlPlayer,
    public svoip::RecorderCallback
{
private:
    virtual void sampled(svoip::EncodedSample* sample, size_t sampleLen,
        size_t frames) {
        play(sample, sampleLen, frames);
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
    boost::scoped_ptr<LocalPlayer> player(new LocalPlayer);
    if (! player->open()) {
        return false;
    }

    boost::scoped_ptr<svoip::Recorder> recorder(new OpenAlRecorder(*player));
    if (! recorder->open()) {
        return false;
    }

    recorder->start();
    player->start();

    ALFWprintf("Press any key to finish.\n");
    while (! ALFWKeyPress()) {
        ::Sleep(1);

        if (! recorder->run()) {
            break;
        }

        if (! player->run()) {
            break;
        }
    }

    recorder->stop();
    player->stop();

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
