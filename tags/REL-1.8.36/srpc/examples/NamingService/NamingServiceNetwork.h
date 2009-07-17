#ifndef SRPC_NAMINGSERVICENETWORK_H
#define SRPC_NAMINGSERVICENETWORK_H

#include <srpc/RpcNetwork.h>
#include <boost/scoped_ptr.hpp>

namespace srpc
{

class StreamBuffer;
class OStream;
class IStream;

} // namespace srpc

namespace ns {

/**
 * @class NamingServiceNetwork
 *
 * RpcNetwork for NamingService
 * - 직렬화를 통한 초단순 네트웍 클래스
 */
class NamingServiceNetwork : public srpc::RpcNetwork
{
public:
    NamingServiceNetwork(bool useBitStream);
    virtual ~NamingServiceNetwork();

    /// @param opposite 상대편(클라이언트 또는 서버)의 RpcNetwork 객체를
    /// 의미한다
    void setOppositeNetwork(NamingServiceNetwork* opposite) {
        opposite_ = opposite;
    }

    void respond(srpc::StreamBuffer& streamBuffer);

    size_t getSentBytes() const {
        return sentBytes_;
    }

    size_t getReceivedBytes() const {
        return receivedBytes_;
    }
private:
    virtual void send(srpc::RpcCommand& command,
        srpc::RpcPacketType packetType, const void* rpcHint = 0);
private:
    NamingServiceNetwork* opposite_;
    boost::scoped_ptr<srpc::StreamBuffer> sendBuffer_;
    boost::scoped_ptr<srpc::StreamBuffer> receiveBuffer_;
    boost::scoped_ptr<srpc::OStream> ostream_;
    boost::scoped_ptr<srpc::IStream> istream_;
    size_t sentBytes_;
    size_t receivedBytes_;
};

} /// namespace ns

#endif // SRPC_NAMINGSERVICENETWORK_H
