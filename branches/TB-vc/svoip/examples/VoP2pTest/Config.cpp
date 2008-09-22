#include "stdafx.h"
#include "Config.h"
#include <iostream>

namespace
{

nsrpc::PeerAddress toPeerAddress(const srpc::String& address)
{
    nsrpc::PeerAddress result;
    srpc::String::size_type pos = address.find(':');
    if (pos != srpc::String::npos) {
        result.ip_ = address.substr(0, pos);
        result.port_ =
            static_cast<srpc::UInt16>(atoi(address.substr(pos + 1).c_str()));
    }
    return result;
}

} // namespace


Config::Config() :
    shouldRecord_(false),
    peerId_(nsrpc::invalidPeerId),
    listeningPort_(0),
    host_(false),
    verbose_(false),
    sleepTime_(0),
    outboundPacketDropRate_(0.0f),
    inboundPacketDropRate_(0.0f),
    minOutboundPacketLatency_(0),
    maxOutboundPacketLatency_(0),
    minInboundPacketLatency_(0),
    maxInboundPacketLatency_(0)
{
}


bool Config::parseArgs(int argc, char* argv[])
{
    if (argc < 2) {
        return false;
    }

    bool isOk = true;
    for (int i = 1; i < argc; ++i) {
        srpc::String arg(argv[i]);
        if (arg == "-?") {
            isOk = false;
            break;
        }
        
        if (arg == "-h") {
            host_ = true;
        }
        else if (arg == "-r") {
            shouldRecord_ = true;
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
        else if (arg.compare(0, 4, "-st=") == 0) {
            sleepTime_ =
                static_cast<srpc::UInt32>(atoi(arg.substr(4).c_str()));
        }
        else if (arg.compare(0, 5, "-spd=") == 0) {
            outboundPacketDropRate_ =
                static_cast<float>(atof(arg.substr(5).c_str()));
        }
        else if (arg.compare(0, 5, "-rpd=") == 0) {
            inboundPacketDropRate_ =
                static_cast<float>(atof(arg.substr(5).c_str()));
        }
        else if (arg.compare(0, 5, "-nsl=") == 0) {
            minOutboundPacketLatency_ =
                static_cast<srpc::UInt32>(atoi(arg.substr(5).c_str()));
        }
        else if (arg.compare(0, 5, "-xsl=") == 0) {
            maxOutboundPacketLatency_ =
                static_cast<srpc::UInt32>(atoi(arg.substr(5).c_str()));
        }
        else if (arg.compare(0, 5, "-nrl=") == 0) {
            minInboundPacketLatency_ =
                static_cast<srpc::UInt32>(atoi(arg.substr(5).c_str()));
        }
        else if (arg.compare(0, 5, "-xrl=") == 0) {
            maxInboundPacketLatency_ =
                static_cast<srpc::UInt32>(atoi(arg.substr(5).c_str()));
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
    std::cout << "Voice Over P2P Test\n" <<
        "USAGE: VoP2pTest [options]\n" <<
        "  -p=<peer-id> unique Peer Id\n" <<
        "  -l=<port> listening UDP port\n" <<
        "  -ra=<address:port> : relay server address\n" <<
        "  -ha=<address:port> : P2P host address(connect to host)\n" <<
        "  -h P2P host\n" <<
        "  -r should record voice?(default: off)\n" <<
        "  -st=<sleep time for each response>: 0~ (default: 0)\n"
        "  -spd=<send packet drop rate> : 0.0 ~ 1.0 (default: 0.0)\n"
        "  -rpd=<recv. packet drop rate> : 0.0 ~ 1.0 (default: 0.0)\n"
        "  -nsl=<min. send packet latency(ms)> : 0 ~ (default: 0)\n"
        "  -xsl=<max. send packet latency(ms)> : 0 ~ (default: 0)\n"
        "  -nrl=<min. recv. packet latency(ms)> : 0 ~ (default: 0)\n"
        "  -xrl=<max. recv. packet latency(ms)> : 0 ~ (default: 0)\n"
        "  -v verbose (default: off)\n" <<
        " ex) P2P host:\n" <<
        "   VoP2pTest -p=1 -l=10000 -ra=111.111.111.111:111 -h\n" <<
        "     P2P peer:\n" <<
        "   VoP2pTest -p=2 -l=20000 -ra=111.111.111.111:111 " <<
                "-ha=222.222.222.222:222\n";
}
