#include "stdafx.h"
#include <nsrpc/p2p/server/SimpleRelayService.h>
#include <nsrpc/ReactorTask.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/LogManager.h>

using namespace nsrpc;
using namespace nsrpc::detail;

int ACE_TMAIN(int argc, ACE_TCHAR* argv[])
{
    if (argc != 2) {
        std::cout << "USAGE: " << argv[0] << " <port_number>\n";
        return EXIT_FAILURE;
    }

    nsrpc::LogManager logManager;
    logManager.verbose();
    //logManager.redirectToFile("P2pTest.log");

    ReactorTask reactorTask(true);
    SimpleRelayService relayService(reactorTask.getReactor(),
        PacketCoderFactory().createForP2p());

    const srpc::UInt16 port = static_cast<srpc::UInt16>(atoi(argv[1]));
    if (! relayService.open(port)) {
        std::cout << "Can't open UDP port(" << port << "\n";
        return EXIT_FAILURE;
    }

    std::cout << "Starting SimpleRelayService at " << port << "...\n" <<
        "Press Ctrl-C to exit.\n";

    reactorTask.start(1);

    reactorTask.wait();

    //relayService.close();
    //reactorTask.stop();

    return EXIT_SUCCESS;
}
