#ifndef NSRPC_P2PRPCNETWORK_H
#define NSRPC_P2PRPCNETWORK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>
#include <nsrpc/p2p/PeerId.h>
#include <srpc/RpcNetwork.h>

namespace srpc
{

class IStream;
class OStream;

} // namespace srpc

namespace nsrpc
{

class MessageBlockStreamBuffer;

namespace detail
{

class PeerNetworkSender;
struct P2pPeerHint;

/** @addtogroup p2p
* @{
*/

/**
 * @class P2pRpcNetwork
 *
 * P2P RpcNetwork
 */
class NSRPC_API P2pRpcNetwork : public srpc::RpcNetwork
{
public:
    P2pRpcNetwork(PeerNetworkSender& networkSender,
        ACE_Message_Block& recvBlock, ACE_Message_Block& sendBlock,
        bool useBitPacking);
    virtual ~P2pRpcNetwork();

    /// 메세지(클라이언트 요청)을 처리한다.
    bool handleMessage(PeerId peerId, const ACE_INET_Addr& peerAddr);
public: // for Test
    virtual void send(srpc::RpcCommand& command,
        srpc::RpcPacketType packetType, const void* rpcHint);
private:
    ACE_Message_Block& initOutputStream();
private:
    PeerNetworkSender& networkSender_;

    bool useBitPacking_;

    boost::scoped_ptr<MessageBlockStreamBuffer> rstreamBuffer_;
    boost::scoped_ptr<srpc::IStream> istream_;

    boost::scoped_ptr<MessageBlockStreamBuffer> wstreamBuffer_;
    boost::scoped_ptr<srpc::OStream> ostream_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // !defined(NSRPC_P2PRPCNETWORK_H)
