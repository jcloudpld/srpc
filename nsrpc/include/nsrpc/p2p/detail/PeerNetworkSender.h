#ifndef NSRPC_PEERNETWORKSENDER_H
#define NSRPC_PEERNETWORKSENDER_H

#include "P2pProtocol.h"
#include "P2pAddress.h"
#include <srpc/RpcPacketType.h>
#include <boost/noncopyable.hpp>

class ACE_Message_Block;

namespace nsrpc
{

namespace detail
{

struct P2pPeerHint;

/** @addtogroup p2p
* @{
*/

/**
 * @class PeerNetworkSender
 *
 * P2P RpcNetwork Helper class
 */
class PeerNetworkSender : public boost::noncopyable
{
public:
    virtual ~PeerNetworkSender() {}

    /**
     * �޼����� �����Ѵ�.
     * @return ���� ���� ����.
     * - ȣ���ڴ� mblock�� �޸� �������� �Ѱ���� �Ѵ�.
     */
    virtual bool sendNow(const PeerIdPair& peerIdPair,
        const AddressPair& addressPair, const ACE_Message_Block& mblock,
        srpc::RpcPacketType packetType, SequenceNumber sequenceNumber,
        PeerTime sentTime = 0) = 0;

    /// ������ �߻��Ͽ���.
    virtual void marshalingErrorOccurred() = 0;

    /**
    * �޼����� �����Ѵ�.
    * @param packetType ��Ŷ ����
    * @param mblock �޼��� ����. �������� �Ѱܾ� �Ѵ�.
    * @param peerHint �ΰ� ����(null�� �ƴϸ� broadcast)
    */
    virtual void sendOutgoingMessage(srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, const P2pPeerHint* peerHint) = 0;

    /// ��Ŷ ����� ũ�⸦ ��´�.
    virtual size_t getPacketHeaderSize() const = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERNETWORKSENDER_H
