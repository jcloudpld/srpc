#ifndef NSRPC_TESTCLIENT_H
#define NSRPC_TESTCLIENT_H

#include <nsrpc/detail/PacketCoder.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/Logger.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <srpc/utility/Endian.h>
#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/OS.h>
#include <string>

/**
 * @class TestClient
 *
 * 테스트용 TCP/IP 클라이언트
 */
class TestClient
{
public:
    TestClient() :
        packetCoder_(nsrpc::PacketCoderFactory().create()) {}
    ~TestClient() {
        close();
    }

    void close() {
        stream_.close();
    }

    // timeout = msec
    bool connect(long timeout, const ACE_INET_Addr& addr)
    {
        ACE_Time_Value connectionTimeout(nsrpc::makeTimeValue(timeout));
        ACE_SOCK_Connector connector;
        if (connector.connect(stream_, addr, &connectionTimeout) == 0) {
            return true;
        }
    NSRPC_LOG_DEBUG(ACE_TEXT("TestClient::connect() - FAILED(%m"));
        return false;
    }

    int sendMessage(const srpc::UInt8* body, int bodySize) {
        ACE_Message_Block msg(
            packetCoder_->getHeaderSize() + bodySize);
        void* wrPtr = msg.wr_ptr();
        *static_cast<srpc::UInt16*>(wrPtr) = srpc::toRpcByteOrder(
            static_cast<srpc::UInt16>(bodySize));
        *(static_cast<srpc::UInt8*>(wrPtr) + sizeof(srpc::UInt16)) =
            static_cast<srpc::UInt8>(nsrpc::mtGeneral);
        msg.wr_ptr(packetCoder_->getHeaderSize());
        const void* bodyPtr = body;
        msg.copy(static_cast<const char*>(bodyPtr), bodySize);
        return stream_.send_n(&msg);
    }

    int recvMessage(void* body, int bodySize) {
        ACE_Time_Value timeout(nsrpc::makeTimeValue(1));
        srpc::UInt16 rpcBodySize = 0;
        if (recv_n(&rpcBodySize, sizeof(rpcBodySize), &timeout) == -1) {
            return -1;
        }
        rpcBodySize = srpc::toRpcByteOrder(rpcBodySize);
        if (rpcBodySize != bodySize) {
            return -1;
        }
        srpc::UInt8 messageType;
        if (recv_n(&messageType, sizeof(messageType), &timeout) == -1) {
            return -1;
        }
        return recv_n(body, bodySize, &timeout);
    }

    bool isDisconnected() {
        ACE_Time_Value timeout(0, 1);
        char buf;
        return stream_.recv(&buf, sizeof(buf), &timeout) == 0;
    }
private:
    ssize_t recv_n(void *buf, size_t len, const ACE_Time_Value *timeout) {
        return stream_.recv_n(buf, len, timeout);
    }
private:
    ACE_SOCK_Stream stream_;
    boost::scoped_ptr<nsrpc::PacketCoder> packetCoder_;
};

#endif // !defined(NSRPC_TESTCLIENT_H)
