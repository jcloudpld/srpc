#ifndef NSRPC_PEERMESSAGEHANDLER_H
#define NSRPC_PEERMESSAGEHANDLER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/p2p/PeerId.h>
#include <srpc/RpcPacketType.h>
#include <boost/noncopyable.hpp>

class ACE_Message_Block;

namespace nsrpc
{

namespace detail
{

struct Message;

/** @addtogroup p2p
* @{
*/

/**
 * @class PeerMessageHandler
 *
 * P2P message handler class
 */
class PeerMessageHandler : public boost::noncopyable
{
public:
    virtual ~PeerMessageHandler() {}

    /**
     * 수신된 메세지를 처리한다.
     * @param mblock 소유권을 넘기지 않아야 한다.
     * @return 정상적으로 처리가 되었는가?
     */
    virtual bool handleIncomingMessage(PeerId peerId,
        const Message& message) = 0;

    /**
     * 해당 Peer의 접속을 확인 했음을 알린다.
     * @param peerId 접속한 peer id.
     */
    virtual bool acknowledgedConnect(PeerId peerId) = 0;

    /**
     * Peer가 접속해제되었다
     * @param peerId 접속해제된 peer id.
     */
    virtual void peerDisconnecting(PeerId peerId) = 0;

    /**
     * 해당 Peer에게 핑 메세지를 보낸다.
     * @param peerId 수신자
     */
    virtual void sendPing(PeerId peerId) = 0;

    /// peerId에게 Ack 메세지를 전송한다
    virtual void sendAcknowledgement(PeerId peerId,
        const Message& message) = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERMESSAGEHANDLER_H
