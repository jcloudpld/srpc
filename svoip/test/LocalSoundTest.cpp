#include "stdafx.h"
#include "OpenAlRecorder.h"
#include "OpenAlPlayer.h"
#include "openal_framework/Framework.h"
#include <boost/scoped_ptr.hpp>

namespace
{

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
    boost::scoped_ptr<svoip::Recorder> recorder(new OpenAlRecorder);
    if (! recorder->open()) {
        return false;
    }

    boost::scoped_ptr<svoip::Player> player(new OpenAlPlayer);
    if (! player->open()) {
        return false;
    }

    recorder->start();
    player->start();

    for (;;) {
        ::Sleep(1);

        if (! recorder->run()) {
            break;
        }

        if (! player->run()) {
            break;
        }

        size_t samples;
        svoip::Sample* sample = recorder->getSample(samples);
        if (! sample) {
            continue;
        }

        player->play(sample, samples);
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
