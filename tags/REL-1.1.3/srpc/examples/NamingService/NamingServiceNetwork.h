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
 * - ����ȭ�� ���� �ʴܼ� ��Ʈ�� Ŭ����
 */
class NamingServiceNetwork : public srpc::RpcNetwork
{
public:
    NamingServiceNetwork(bool useBitStream);
    virtual ~NamingServiceNetwork();

    /// @param opposite �����(Ŭ���̾�Ʈ �Ǵ� ����)�� RpcNetwork ��ü��
    /// �ǹ��Ѵ�
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
