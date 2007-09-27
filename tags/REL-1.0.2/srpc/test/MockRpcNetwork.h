#ifndef SRPC_MOCKRPCNETWORK_H
#define SRPC_MOCKRPCNETWORK_H

#include "DummyStreamBuffer.h"
#include <srpc/RpcNetwork.h>
#include <srpc/detail/RpcCommand.h>
#include <srpc/detail/OBitStream.h>

/**
 * @class MockRpcNetwork
 */
class MockRpcNetwork : public srpc::RpcNetwork
{
public:
    MockRpcNetwork() :
        ostream_(buffer_),
        sendFailed_(false) {}

    void setMarshalingError() {
        buffer_.setPushError();
    }

    srpc::OBitStream& getOutputStream() {
        return ostream_;
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
        srpc::RpcPacketType /*packetType*/, const void* rpcHint)  {
        try {
            command.marshal(ostream_);
            sentRpcHint_ = static_cast<int>(reinterpret_cast<size_t>(rpcHint));
            //sendFailed_ = false;
        }
        catch (const srpc::Exception&) {
            sendFailed_ = true;
        }
    }
private:
    DummyStreamBuffer buffer_;
    srpc::OBitStream ostream_;
    bool sendFailed_;
    int sentRpcHint_;
};

#endif // SRPC_MOCKRPCNETWORK_H
