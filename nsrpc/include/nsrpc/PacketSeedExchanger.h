#ifndef NSRPC_PACKETSEEDEXCHANGER_H
#define NSRPC_PACKETSEEDEXCHANGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "detail/rpc_interface/SecurityService.h"
#include "nsrpc.h"
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>

namespace nsrpc
{

class PacketSeedExchangerCallback;
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
        callback_(0),
        packetCoder_(0) {}

    /// @internal
    void initialize(PacketSeedExchangerCallback& callback,
        PacketCoder& packetCoder, SessionRpcNetwork& rpcNetwork);

    /// 시드값 교환을 위해 공개키 알고리즘을 사용할 경우 공개키를 교환한다
    virtual void exchangePublicKey() = 0;

    /// 서버에서 주기적으로 시드값을 교환한다
    virtual void exchangeNextSeed() = 0;
protected:
    PacketSeedExchangerCallback& getCallback() {
        assert(callback_ != 0);
        return *callback_;
    }

    PacketCoder& getPacketCoder() {
        assert(packetCoder_ != 0);
        return *packetCoder_;
    }
private:
    PacketSeedExchangerCallback* callback_;
    PacketCoder* packetCoder_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_PACKETSEEDEXCHANGER_H)
