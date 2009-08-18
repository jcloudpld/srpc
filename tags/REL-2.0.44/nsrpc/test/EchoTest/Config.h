#ifndef ECHOTEST_CONFIG_H
#define ECHOTEST_CONFIG_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <srpc/Types.h>
#include <srpc/StringTypes.h>
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
    srpc::String getConnectHost() const {
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
    srpc::String getProactorType() const {
        return proactorType_;
    }
    bool isVerbose() const {
        return verbose_;
    }
private:
    void printUsage();
private:
    srpc::String mode_;
    srpc::String connectHost_;
    srpc::UInt16 port_;
    srpc::UInt32 threads_;
    srpc::UInt32 connections_;
    srpc::UInt32 echoCount_;
    srpc::UInt32 blockSize_;
    bool useRpc_;
    srpc::String streamType_;
    srpc::String proactorType_;
    bool verbose_;
};

#endif // ECHOTEST_CONFIG_H
