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
     * ���ŵ� �޼����� ó���Ѵ�.
     * @param mblock �������� �ѱ��� �ʾƾ� �Ѵ�.
     * @return ���������� ó���� �Ǿ��°�?
     */
    virtual bool handleIncomingMessage(PeerId peerId,
        const Message& message) = 0;

    /**
     * �ش� Peer�� ������ Ȯ�� ������ �˸���.
     * @param peerId ������ peer id.
     */
    virtual bool acknowledgedConnect(PeerId peerId) = 0;

    /**
     * Peer�� ���������Ǿ���
     * @param peerId ���������� peer id.
     */
    virtual void peerDisconnecting(PeerId peerId) = 0;

    /**
     * �ش� Peer���� �� �޼����� ������.
     * @param peerId ������
     */
    virtual void sendPing(PeerId peerId) = 0;

    /// peerId���� Ack �޼����� �����Ѵ�
    virtual void sendAcknowledgement(PeerId peerId,
        const Message& message) = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERMESSAGEHANDLER_H
