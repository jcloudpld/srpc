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
 * ���� ������� ���� Peer�� ������ �޼����� �����ϰ� ó���Ѵ�.
 * - Manager�� �̸��� ���� ������ ���� ���� ���� ����� ����. ��.��
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
