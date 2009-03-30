#include "stdafx.h"
#include "Config.h"
#include "EchoServer.h"
#include "EchoClient.h"

#if defined(NSRPC_HAS_PROACTOR)

#include <nsrpc/ProactorTask.h>
#include <nsrpc/ProactorFactory.h>
#include <nsrpc/CachedSessionManager.h>
#include <nsrpc/SessionAcceptor.h>
#include <nsrpc/SessionConnector.h>

#include <nsrpc/utility/SystemUtil.h>
#include <nsrpc/utility/LogManager.h>
#include <ace/Profile_Timer.h>
#include <iostream>
#include <conio.h>

#endif // #if defined(NSRPC_HAS_PROACTOR)

#if defined(NSRPC_HAS_PROACTOR)

using namespace nsrpc;

// Let's go!
void run(const Config& config)
{
    ProactorTask proactorTask(
        ProactorFactory::create(toProactorType(config.getProactorType())),
        true);
    if (! proactorTask.start(config.getThreads())) {
        return;
    }

    size_t poolSize = 5;
    if (config.getConnections() > poolSize) {
        poolSize = config.getConnections();
    }

    // server setup
    ServerSessionFactory serverSessionFactory(config,
        proactorTask.getProactor(), config.useBitPacking());
    boost::scoped_ptr<SessionManager> serverSessionManager(
        new CachedSessionManager("ServerSessionManager", poolSize,
            serverSessionFactory));
    serverSessionManager->initialize();
    SessionAcceptor acceptor(*serverSessionManager);

    // client setup
    ClientSessionFactory clientSessionFactory(config,
        proactorTask.getProactor(), config.useBitPacking());
    boost::scoped_ptr<SessionManager> clientSessionManager(
        new CachedSessionManager("ClientSessionManager", poolSize,
            clientSessionFactory));
    clientSessionManager->initialize();
    SessionConnector connector(*clientSessionManager);

    // start
    ACE_Profile_Timer profiler;

    if (config.isServer()) {
        if (! acceptor.open(ACE_INET_Addr(config.getPort()),
            proactorTask.getProactor())) {
            return;
        }
        acceptor.start();
        std::cout << "Running Server at " << ACE_LOCALHOST << ":" <<
            config.getPort() << "...\n";
    }

    if (config.isClient()) {
        profiler.start();
        ACE_INET_Addr addr(config.getPort(), config.getConnectHost().c_str());
        const size_t clientCount = connector.start(addr,
            proactorTask.getProactor(), config.getConnections());
        // 클라이언트가 접속할 수 있는 여유 시간을 준다.
        if (clientCount > 0) {
            ACE_Time_Value now = ACE_OS::gettimeofday();
            while (clientSessionManager->isSafeToDelete()) {
                if ((ACE_OS::gettimeofday() - now).sec() >= 1) {
                    break;
                }
                nsrpc::pause(1);
            }
        }
    }

    // running
    if (config.isClient()) {
        clientSessionManager->wait();

        // print result
        profiler.stop();
        ACE_Profile_Timer::ACE_Elapsed_Time et;
        profiler.elapsed_time(et);
        std::cout << "Elapsed Time: " <<
            "real time = " << et.real_time << " secs, " <<
            "user time = " << et.user_time << " secs, " <<
            "system time = " << et.system_time << " secs\n";
    }
    else if (config.isServer()) { // server only
        for (;;) {
            if (_kbhit()) {
                break;
            }
            nsrpc::pause(1000);
        }
    }

    // finishing
    connector.stop();
    connector.wait();

    acceptor.close();
    acceptor.wait();

    serverSessionManager->cancel();
    serverSessionManager->wait();

    proactorTask.stop();

    serverSessionManager.reset();
    clientSessionManager.reset();
}

#endif // #if defined(NSRPC_HAS_PROACTOR)

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    Config config;
    if (! config.parseArgs(argc, argv)) {
        return -1;
    }

#if defined(NSRPC_HAS_PROACTOR)

    nsrpc::LogManager logManager;
    if (config.isVerbose()) {
        logManager.verbose();
        //logManager.redirectToFile("EchoTest.log");
    }
    else {
        logManager.noDebug();
    }

    run(config);

#endif // #if defined(NSRPC_HAS_PROACTOR)

    return 0;
}
