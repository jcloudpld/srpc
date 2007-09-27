#include "stdafx.h"
#include "Config.h"
#include <iostream>

nsrpc::PeerAddress toPeerAddress(const std::string& address)
{
    nsrpc::PeerAddress result;
    std::string::size_type pos = address.find(':');
    if (pos != std::string::npos) {
        result.ip_ = address.substr(0, pos);
        result.port_ =
            static_cast<srpc::UInt16>(atoi(address.substr(pos + 1).c_str()));
    }
    return result;
}


bool Config::parseArgs(int argc, char* argv[])
{
    if (argc < 2) {
        return false;
    }

    bool isOk = true;
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "-?") {
            isOk = false;
            break;
        }
        
        if (arg == "-h") {
            host_ = true;
        }
        else if (arg == "-v") {
            verbose_ = true;
        }
        else if (arg.compare(0, 3, "-p=") == 0) {
            peerId_ = static_cast<nsrpc::PeerId>(atoi(arg.substr(3).c_str()));
        }
        else if (arg.compare(0, 3, "-l=") == 0) {
            listeningPort_ =
                static_cast<srpc::UInt16>(atoi(arg.substr(3).c_str()));
        }
        else if (arg.compare(0, 4, "-ra=") == 0) {
            nsrpc::PeerAddress address = toPeerAddress(arg.substr(4));
            if (! address.isValid()) {
                isOk = false;
                break;
            }
            relayServerAddress_ = address;
        }
        else if (arg.compare(0, 4, "-ha=") == 0) {
            nsrpc::PeerAddress address = toPeerAddress(arg.substr(4));
            if (! address.isValid()) {
                isOk = false;
                break;
            }
            hostAddresses_.push_back(address);
        }
        else {
            std::cerr << "Unknown option '" << arg << "', ignoring it.\n";
        }
    }

    if (! nsrpc::isValidPeerId(peerId_)) {
        isOk = false;
    }
    else if (listeningPort_ == 0) {
        isOk = false;
    }
    else if (! isHost()) {
        if (hostAddresses_.empty()) {
            isOk = false;
        }
    }

    return isOk;
}


void Config::printUsage()
{
    std::cout << "USAGE: P2pTest [options]\n" <<
        "  -p=<peer-id> unique Peer Id\n" <<
        "  -l=<port> listening UDP port\n" <<
        "  -ra=<address:port> : relay server address\n" <<
        "  -ha=<address:port> : P2P host address(connect to host)\n" <<
        "  -h P2P host\n" <<
        "  -v verbose (default: off)\n" <<
        " ex) P2P host:\n" <<
        "   P2pTest -p=1 -l=10000 -ra=111.111.111.111:111 -h\n" <<
        "     P2P peer:\n" <<
        "   P2pTest -p=2 -l=20000 -ra=111.111.111.111:111 " <<
                "-ha=222.222.222.222:222\n";
}
