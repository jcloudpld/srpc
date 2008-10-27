#ifndef NSRPC_RPCNETWORKFORBROADCAST_H
#define NSRPC_RPCNETWORKFORBROADCAST_H

#include <nsrpc/detail/MessageBlockProvider.h>
#include <nsrpc/config/config.h>
#include <srpc/RpcNetwork.h>
#include <srpc/RpcForwarder.h>
#include <boost/scoped_ptr.hpp>

class ACE_Message_Block;

namespace srpc
{

class IStream;
class OStream;

} // namespace srpc

namespace nsrpc
{

class PacketCoder;
class PacketCoderFactory;
class SynchMessageBlockManager;
class MessageBlockStreamBuffer;

/** @addtogroup session
* @{
*/

/**
 * @class RpcNetworkForBroadcast
 *
 * RpcNetwork for broadcasting
 */
class NSRPC_API RpcNetworkForBroadcast :
    public srpc::RpcNetwork,
    public srpc::RpcForwarder,
    private MessageBlockProvider
{
public:
    RpcNetworkForBroadcast();
    virtual ~RpcNetworkForBroadcast();

    /// @return send block.
    void* acquireBlock();

protected:
    void initialize(bool useBitPacking,
        PacketCoderFactory& packetCoderFactory);

private:
    virtual void registerRpcReceiver(srpc::RpcReceiver* /*receiver*/) {
        assert(false && "DON'T CALL ME!");
    }
    virtual void unregisterRpcReceiver(srpc::RpcReceiver* /*receiver*/) {
        assert(false && "DON'T CALL ME!");
    }
    virtual void send(srpc::RpcCommand& command,
        srpc::RpcPacketType packetType, const void* rpcHint);

    // = MessageBlockProvider overriding
    virtual ACE_Message_Block& acquireSendBlock();
    virtual ACE_Message_Block& acquireRecvBlock();

private:
    ACE_Message_Block* marshal(srpc::RpcCommand& command);
    ACE_Message_Block* initOutputStream();
    void releaseSendBlock();

private:
    bool useBitPacking_;
    boost::scoped_ptr<PacketCoder> packetCoder_;
    boost::scoped_ptr<SynchMessageBlockManager> messageBlockManager_;
    boost::scoped_ptr<MessageBlockStreamBuffer> wstreamBuffer_;
    boost::scoped_ptr<srpc::OStream> ostream_;

    ACE_Message_Block* sendBlock_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // !defined(NSRPC_RPCNETWORKFORBROADCAST_H)
