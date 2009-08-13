#ifndef NSRPC_RELAYSERVICEHANDLER_H
#define NSRPC_RELAYSERVICEHANDLER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/p2p/detail/P2pProtocol.h>
#include <nsrpc/p2p/PeerId.h>
#include <srpc/RpcPacketType.h>
#include <boost/noncopyable.hpp>

class ACE_Message_Block;

namespace nsrpc
{

namespace detail
{

/** @addtogroup p2p
* @{
*/

/**
 * @class RelayServiceHandler
 *
 * P2P relay service handler
 */
class RelayServiceHandler : public boost::noncopyable
{
public:
    virtual ~RelayServiceHandler() {}

    /// �޼��� ���� ��´�.
    virtual ACE_Message_Block* acquire(size_t blockSize) = 0;

    /// ��Ŷ ������
    virtual void relayed(PeerId peerId, const ACE_INET_Addr& peerAddress,
        ACE_Message_Block* mblock, srpc::RpcPacketType packetType,
        SequenceNumber sequenceNumber, srpc::UInt32 sentTime) = 0;
};

/** @} */ // addtogroup p2p

} // namespace detail

} // namespace nsrpc

#endif // NSRPC_RELAYSERVICEHANDLER_H
