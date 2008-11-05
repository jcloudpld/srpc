#ifndef NSRPC_PACKETSEEDEXCHANGER_H
#define NSRPC_PACKETSEEDEXCHANGER_H

#include "detail/rpc_interface/SecurityService.h"
#include "config/config.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>

namespace nsrpc
{

class PacketCoder;
class SessionRpcNetwork;

/**
 * @class PacketSeedExchanger
 * 클라이언트/서버 간에 패킷 암호화 시드 값을 교환한다
 */
class NSRPC_API PacketSeedExchanger :
    protected srpc::RpcReceiver, protected srpc::RpcForwarder,
    public RpcSecurityService
{
    DECLARE_SRPC_EVENT_DISPATCHER(PacketSeedExchanger);
public:
    PacketSeedExchanger() :
        packetCoder_(0) {}

    /// @internal
    void initialize(PacketCoder* packetCoder,
        SessionRpcNetwork* rpcNetwork);

    /// 클라이언트가 서버에 접속했을 경우 서버에서 초기 시드값을 교환한다
    virtual void exchangeFirstSeed() = 0;

    /// 서버에서 주기적으로 시드값을 교환한다
    virtual void exchangeNextSeed() = 0;
protected:
    PacketCoder& getPacketCoder() {
        return *packetCoder_;
    }
private:
    PacketCoder* packetCoder_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETSEEDEXCHANGER_H)
