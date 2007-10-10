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
 * Ŭ���̾�Ʈ/���� ���� ��Ŷ ��ȣȭ �õ� ���� ��ȯ�Ѵ�
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

    /// Ŭ���̾�Ʈ�� ������ �������� ��� �������� �ʱ� �õ尪�� ��ȯ�Ѵ�
    virtual void exchangeFirstSeed() = 0;

    /// �������� �ֱ������� �õ尪�� ��ȯ�Ѵ�
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
