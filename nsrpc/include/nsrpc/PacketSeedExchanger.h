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
 * Ŭ���̾�Ʈ/���� ���� ��Ŷ ��ȣȭ �õ� ���� ��ȯ�Ѵ�
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

    /// �õ尪 ��ȯ�� ���� ����Ű �˰����� ����� ��� ����Ű�� ��ȯ�Ѵ�
    virtual void exchangePublicKey() = 0;

    /// �������� �ֱ������� �õ尪�� ��ȯ�Ѵ�
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
