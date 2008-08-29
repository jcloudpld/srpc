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
     * 메세지를 전송한다.
     * @return 전송 성공 여부.
     * - 호출자는 mblock의 메모리 소유권을 넘겨줘야 한다.
     */
    virtual bool sendNow(const PeerIdPair& peerIdPair,
        const AddressPair& addressPair, const ACE_Message_Block& mblock,
        srpc::RpcPacketType packetType, SequenceNumber sequenceNumber,
        PeerTime sentTime = 0) = 0;

    /// 에러가 발생하였다.
    virtual void marshalingErrorOccurred() = 0;

    /**
    * 메세지를 전송한다.
    * @param packetType 패킷 형태
    * @param mblock 메세지 버퍼. 소유권을 넘겨야 한다.
    * @param peerHint 부가 정보(null이 아니면 broadcast)
    */
    virtual void sendOutgoingMessage(srpc::RpcPacketType packetType,
        ACE_Message_Block* mblock, const P2pPeerHint* peerHint) = 0;

    /// 패킷 헤더의 크기를 얻는다.
    virtual size_t getPacketHeaderSize() const = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_PEERNETWORKSENDER_H
