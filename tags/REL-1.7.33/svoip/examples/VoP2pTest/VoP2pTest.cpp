#include "stdafx.h"
#include "Config.h"
#include "Peer.h"
#include "openal_framework/Framework.h"
#include <nsrpc/utility/LogManager.h>

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

} // namespace

// Let's go!
void run(const Config& config)
{
    std::cout << "* Press Ctrl-C to exit.\n";

    Peer peer(config);

    const bool isReady = peer.ready();
    if (! isReady) {
        return;
    }

    for (;;) {
        const bool isOk = peer.tick();
        if (! isOk) {
            break;
        }
    }
}


int main(int argc, char* argv[])
{
    Config config;
    if (! config.parseArgs(argc, argv)) {
        config.printUsage();
        return EXIT_FAILURE;
    }

    nsrpc::LogManager::redirectToOStream(&std::cout);

    if (config.isVerbose()) {
        nsrpc::LogManager::verbose();
    }
    else {
        nsrpc::LogManager::noDebug();
    }

    if (! initializeOpenAl()) {
        return EXIT_FAILURE;
    }

    run(config);

    shutdownOpenAl();

    return EXIT_SUCCESS;
}
