#ifndef SRPC_MOCKRPCNETWORK_H
#define SRPC_MOCKRPCNETWORK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "DummyStreamBuffer.h"
#include <srpc/RpcNetwork.h>
#include <srpc/detail/RpcCommand.h>
#include <srpc/OStream.h>

/**
 * @class MockRpcNetwork
 */
class MockRpcNetwork : public srpc::RpcNetwork
{
public:
    MockRpcNetwork();
    virtual ~MockRpcNetwork();

    void setMarshalingError() {
        buffer_.setPushError();
    }

    srpc::OStream& getOutputStream() {
        return *ostream_;
    }

    srpc::StreamBuffer& getStreamBuffer() {
        return buffer_;
    }

    bool sendFailed() const {
        return sendFailed_;
    }

    int getSentRpcHint() const {
        return sentRpcHint_;
    }
private:
    virtual void send(srpc::RpcCommand& command,
        srpc::RpcPacketType packetType, const void* rpcHint);
private:
    DummyStreamBuffer buffer_;
    boost::scoped_ptr<srpc::OStream> ostream_;
    bool sendFailed_;
    int sentRpcHint_;
};

#endif // SRPC_MOCKRPCNETWORK_H
