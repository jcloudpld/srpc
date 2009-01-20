#ifndef NSRPC_SIMPLERELAYSERVICE_H
#define NSRPC_SIMPLERELAYSERVICE_H

#include "../rpc_interface/StunService.h"
#include "../rpc_interface/RelayService.h"
#include "../detail/P2pEndpoint.h"
#include "../detail/P2pRpcNetwork.h"
#include "../detail/PeerNetworkSender.h"
#include "../detail/PeerNotifier.h"
#include "../detail/PeerCipherKeyCache.h"
#include "../../config/config.h"

namespace nsrpc
{

class PacketCoder;

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class SimpleRelayService
 * RelayServer For Test
 */
class NSRPC_API SimpleRelayService :
    public RpcStunService, public RpcRelayService,
    private srpc::RpcReceiver, private srpc::RpcForwarder,
    private PeerNotifier,
    private PeerNetworkSender
{
    DECLARE_SRPC_EVENT_DISPATCHER(SimpleRelayService);
public:
    SimpleRelayService(ACE_Reactor* reactor, PacketCoder* packetCoder,
        bool useBitPacking = true);
    virtual ~SimpleRelayService();

    /**
     * @param cipherKey If the key is null string, a default key is used.
     */
    bool open(srpc::UInt16 port, const srpc::String& cipherKey = "");
    void close();
public:
    // = for Test
    PeerIdPair getLastRelayPeerIdPair() const {
        return lastRelayPeerIdPair_;
    }
public:
    // = RpcStunService
    DECLARE_SRPC_P2P_METHOD_0(rpcResolve);
    DECLARE_SRPC_METHOD_2(rpcResolved,
        srpc::RShortString, ipAddress, srpc::RUInt16, port);

    // = RpcRelayService
    DECLARE_SRPC_P2P_METHOD_6(rpcRelay,
        RPeerIdPair, peerIdPair, RAddress, peerAddress,
        RMessageBuffer, messageBlock, srpc::RRpcPacketType, packetType,
        RSequenceNumber, sequenceNumber, RRelativeTime, sentTime);
    DECLARE_SRPC_METHOD_5(rpcRelayed,
        RPeerId, peerId, RMessageBuffer, messageBlock,
        srpc::RRpcPacketType, packetType,
        RSequenceNumber, sequenceNumber,
        RRelativeTime, sentTime);
private:
    // = PeerNotifier overriding
    virtual void onMessageArrived(const ACE_INET_Addr& peerAddress);

    // = PeerNetworkSender overriding
    virtual bool sendNow(const PeerIdPair& peerIdPair,
        const AddressPair& addressPair, const ACE_Message_Block& mblock,
        srpc::RpcPacketType packetType, SequenceNumber sequenceNumber,
        srpc::UInt32 sentTime);
    virtual void marshalingErrorOccurred();
    virtual void sendOutgoingMessage(srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, const P2pPeerHint* peerHint);
    virtual size_t getPacketHeaderSize() const;
private:
    ACE_Reactor* reactor_;
    boost::scoped_ptr<PacketCoder> packetCoder_;
    ACE_Message_Block* recvBlock_;
    ACE_Message_Block* sendBlock_;
    P2pEndpoint endpoint_;
    P2pRpcNetwork rpcNetwork_;
    SequenceNumber outgoingUnreliableSequenceNumber_;

    PeerIdPair lastRelayPeerIdPair_;

    PeerCipherKeyCache peerCipherKeys_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_SIMPLERELAYSERVICE_H
