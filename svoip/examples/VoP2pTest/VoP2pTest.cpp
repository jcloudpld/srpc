#include "stdafx.h"
#include "Config.h"
#include "Peer.h"
#include "openal_framework/Framework.h"
#include <nsrpc/utility/LogManager.h>

namespace
{

class VoP2pLogEventHandler : public nsrpc::LogEventHandler
{
public:
    virtual void onLog(const char* log) {
        std::cout << log;
    }
};

void printLogs(nsrpc::LogRepository& logRepository)
{
#ifdef USE_QUEUEING_LOG
    while (logRepository.hasLog()) {
        std::cout << logRepository.getLog();
    }
#else
    logRepository;
#endif
}


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
void run(const Config& config, nsrpc::LogRepository& logRepository)
{
    std::cout << "* Press Ctrl-C to exit.\n";

    Peer peer(config);

    const bool isReady = peer.ready();
    printLogs(logRepository);
    if (! isReady) {
        return;
    }

    for (;;) {
        const bool isOk = peer.tick();
        printLogs(logRepository);
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

    VoP2pLogEventHandler logEventHandler;
    boost::scoped_ptr<nsrpc::LogRepository> logRepository_(
        nsrpc::LogRepositoryFactory::create(false, &logEventHandler));
    nsrpc::LogManager::redirectToCallback(*logRepository_);
    //nsrpc::LogManager::redirectToOStream(&std::cout);

    if (config.isVerbose()) {
        nsrpc::LogManager::verbose();
    }
    else {
        nsrpc::LogManager::noDebug();
    }

    if (! initializeOpenAl()) {
        return EXIT_FAILURE;
    }

    run(config, *logRepository_);

    shutdownOpenAl();

    return EXIT_SUCCESS;
}
