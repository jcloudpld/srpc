#ifndef ECHOTEST_CONFIG_H
#define ECHOTEST_CONFIG_H

#include <srpc/Types.h>
#include <string>

/**
 * @class Config
 * ÄÁÇÈ.
 */
class Config
{
public:
    Config();

    bool parseArgs(int argc, ACE_TCHAR* argv[]);

    bool isServer() const {
        return mode_ == "server" || mode_ == "both";
    }
    bool isClient() const {
        return mode_ == "client" || mode_ == "both";
    }
    std::string getConnectHost() const {
        return connectHost_;
    }
    srpc::UInt16 getPort() const {
        return port_;
    }
    srpc::UInt32 getThreads() const {
        return threads_;
    }
    srpc::UInt32 getConnections() const {
        return connections_;
    }
    srpc::UInt32 getEchoCount() const {
        return echoCount_;
    }
    srpc::UInt32 getBlockSize() const {
        return blockSize_;
    }
    bool useRpc() const {
        return useRpc_;
    }
    bool useBitPacking() const {
        return streamType_ == "bit";
    }
    std::string getProactorType() const {
        return proactorType_;
    }
    bool isVerbose() const {
        return verbose_;
    }
private:
    void printUsage();
private:
    std::string mode_;
    std::string connectHost_;
    srpc::UInt16 port_;
    srpc::UInt32 threads_;
    srpc::UInt32 connections_;
    srpc::UInt32 echoCount_;
    srpc::UInt32 blockSize_;
    bool useRpc_;
    std::string streamType_;
    std::string proactorType_;
    bool verbose_;
};

#endif // ECHOTEST_CONFIG_H
