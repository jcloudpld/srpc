#include "stdafx.h"
#include "NamingServiceNetwork.h"
#include <srpc/detail/RpcCommand.h>
#include <srpc/StreamFactory.h>
#include <srpc/detail/VectorStreamBuffer.h>

namespace ns {

NamingServiceNetwork::NamingServiceNetwork(bool useBitStream) :
    opposite_(0),
    sendBuffer_(new srpc::VectorStreamBuffer),
    receiveBuffer_(new srpc::VectorStreamBuffer),
    sentBytes_(0),
    receivedBytes_(0)
{
    const srpc::StreamFactory::StreamType streamType = useBitStream ?
        srpc::StreamFactory::stBit : srpc::StreamFactory::stByte;
    
    ostream_.reset(srpc::StreamFactory::createOStream(
        true, streamType, *sendBuffer_).release());
    istream_.reset(srpc::StreamFactory::createIStream(
        true, streamType, *receiveBuffer_).release());
}


NamingServiceNetwork::~NamingServiceNetwork()
{
}


void NamingServiceNetwork::send(srpc::RpcCommand& command,
    srpc::RpcPacketType /*packetType*/, const void*)
{
    assert(opposite_ != 0);
    assert(sendBuffer_->empty());

    // RPC 파라미터를 스트림으로 변환한 후
    try {
        command.marshal(*ostream_);
    }
    catch (const srpc::Exception& e) {
        std::cerr << e.what() << "\n";
        return;
    }

    // 상대편에게 전송한다
    opposite_->respond(*sendBuffer_);
    sentBytes_ += sendBuffer_->size();;
    sendBuffer_->reset();
}


void NamingServiceNetwork::respond(srpc::StreamBuffer& clientBuffer)
{
    try {
        receiveBuffer_->copyFrom(&(clientBuffer.front()), clientBuffer.size());
        onReceive(*istream_);
        receivedBytes_ += clientBuffer.size();
    }
    catch (const srpc::UnknownRpcMethodException& e) {
        std::cerr << "RPC error: " << e.what() << "\n";
    }
    catch (const srpc::StreamingException& e) {
        std::cerr << "Streaming error: " << e.what() << "\n";
    }
}

} // namespace ns

