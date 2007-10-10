#ifndef NSRPC_ANONYMOUSMESSAGEMANAGER_H
#define NSRPC_ANONYMOUSMESSAGEMANAGER_H

#include "P2pMessages.h"
#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace detail
{

class PeerMessageHandler;

/** @addtogroup p2p
* @{
*/

/**
 * @class AnonymousMessageManager
 * 아직 연결되지 않은 Peer로 부터의 메세지를 저장하고 처리한다.
 * - Manager란 이름이 정말 싫은데 딱히 생각 나는 대안이 없다. ㅠ.ㅠ
 */
class AnonymousMessageManager : public boost::noncopyable
{
    typedef MessageSet<UnknownReliableMessage> UnknownReliableMessages;
    typedef MessageSet<UnknownUnreliableMessage> UnknownUnreliableMessages;
public:
    AnonymousMessageManager(PeerMessageHandler& messageHandler) :
        messageHandler_(messageHandler) {}

    void reset();

    void putIncomingMessage(const P2pPacketHeader& header,
        const ACE_INET_Addr& peerAddress, ACE_Message_Block* mblock);
    void handleIncomingMessages();
private:
    void handleIncomingReliableMessages();
    void handleIncomingUnreliableMessages();
    void handleIncomingMessage(PeerId peerId, Message& msg,
        srpc::RpcPacketType packetType);
private:
    PeerMessageHandler& messageHandler_;

    UnknownReliableMessages incomingUnknownReliableMessages_;
    UnknownUnreliableMessages incomingUnknownUnreliableMessages_;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_ANONYMOUSMESSAGEMANAGER_H
