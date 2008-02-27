#include "stdafx.h"
#include "Config.h"
#include <ace/Get_Opt.h>
#include <iostream>

Config::Config() :
    mode_("both"),
    connectHost_(ACE_LOCALHOST),
    port_(ACE_DEFAULT_SERVER_PORT),
    threads_(1),
    connections_(10),
    echoCount_(100),
    blockSize_(128),
    useRpc_(false),
    streamType_("bit"),
    verbose_(false)
{
#ifdef _WIN32
    proactorType_ = "iocp";
#endif
}


bool Config::parseArgs(int argc, ACE_TCHAR* argv[])
{
    ACE_Get_Opt getOpt(argc, argv, ACE_TEXT("a:c:e:k:m:n:p:s:t:hrv"));

    bool isOk = true;
    int c;
    while ((c = getOpt()) != EOF) {
        switch (c) {
        case 'a':
            connectHost_ = getOpt.opt_arg();
            break;
        case 'c':
            connections_ = ACE_OS::atoi(getOpt.opt_arg());
            break;
        case 'e':
            echoCount_ = ACE_OS::atoi(getOpt.opt_arg());
            break;
        case 'k':
            blockSize_ = ACE_OS::atoi(getOpt.opt_arg());
            break;
        case 'm':
            mode_ = getOpt.opt_arg();
            break;
        case 'n':
            threads_ = ACE_OS::atoi(getOpt.opt_arg());
            break;
        case 'p':
            port_ =
                static_cast<srpc::UInt16>(ACE_OS::atoi(getOpt.opt_arg()));
            break;
        case 'r':
            useRpc_ = true;
            break;
        case 's':
            streamType_ = getOpt.opt_arg();
            break;
        case 't':
            proactorType_ = getOpt.opt_arg();
            break;
        case 'v':
            verbose_ = true;
            break;
        case 'h':
        default:
            isOk = false;
            break;
        }
    }

    if ((! isServer()) && (! isClient())) {
        isOk = false;
    }

    if ((streamType_ != "bit") && (streamType_ != "byte")) {
        isOk = false;
    }

    if (! isOk) {
        printUsage();
    }
    return isOk;
}


void Config::printUsage()
{
    std::cout << "USAGE: EchoTest [options]\n" <<
        "  -m client, server or both\n" <<
        "      server : server only\n" <<
        "      client : client only\n" <<
        "      both : server/client (default)\n" <<
        "  Client options:\n" <<
        "    -a host to connect (default: ACE_LOCALHOST)\n" <<
        "    -c connection count (default: 10)\n" <<
        "    -e echo count (default: 100)\n" <<
        "    -k data block size to send/receive (default: 128)\n" <<
        "  Common options:\n" <<
        "    -p port to connect/listen (default: ACE_DEFAULT_SERVER_PORT)\n" <<
        "    -n number threads in thread pool (default: 1)\n" <<
        "    -r use RPC (default: false)\n" <<
        "    -s stream type (with only -r)\n" <<
        "        bit : Bit Stream (default)\n" <<
        "        byte : Byte Stream\n" <<
        "    -t Proactor type\n" <<
        "        iocp : WIN32 - IOCP\n" <<
        "        select : SELECT - select()\n" <<
        "        poll : POLL - poll()\n" <<
        "        epoll : EPOLL - Linux epoll_wait()\n" <<
        "        linuxrt : Linux RT - Linux waitsiginfo()\n" <<
        "        linux : Linux - io_getevents()\n" <<
        "        aiocb : POSIX AIOCB - aiosuspend()\n" <<
        "        sig : POSIX SIG - sigwaitinfo()\n" <<
        "        callback : POSIX CALLBACK - SGI style aio callbacks\n" <<
        "        sun : SUN - Sun aiowait()\n" <<
        "        devpoll : DEVPOLL - Sun /dev/poll\n" <<
        "        sunport : SUNPORT - Sun port_get()\n" <<
        "    -v verbose (default: off)\n" <<
        "    -h show this message\n" <<
        " ex) server only: EchoTest -m server -t iocp\n" <<
        "     client only: EchoTest -m client -t select -n 2\n" <<
        "     server/client: EchoTest -t epoll -e 2000 -k 512\n";
}
