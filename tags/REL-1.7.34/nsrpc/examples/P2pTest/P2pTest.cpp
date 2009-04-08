#include "stdafx.h"
#include "Config.h"
#include "Peer.h"
#include <nsrpc/utility/LogManager.h>

namespace
{

class P2pLogEventHandler : public nsrpc::LogEventHandler
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
    SRPC_UNUSED_ARG(logRepository);
#endif
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

    peer.chat("Let's party~");

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

    P2pLogEventHandler logEventHandler;
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

    run(config, *logRepository_);

    return EXIT_SUCCESS;
}
