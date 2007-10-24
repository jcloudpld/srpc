#ifndef NSRPC_RPCSESSIONCONFIG_H
#define NSRPC_RPCSESSIONCONFIG_H

#include "SessionConfig.h"

namespace nsrpc
{

class SessionRpcNetwork;
class PacketSeedExchanger;

/**
 * @struct RpcSessionConfig
 */
struct RpcSessionConfig : public SessionConfig
{
    /**
    * @param rpcNetwork 메모리 소유권을 넘겨야 한다
    * @param seedExchanger 메모리 소유권을 넘겨야 한다
    */
    RpcSessionConfig(SessionDestroyer* sessionDestroyer,
        SynchMessageBlockManager* messageBlockManager,
        PacketCoder* packetCoder, NSRPC_Proactor* proactor,
        SessionRpcNetwork* rpcNetwork, PacketSeedExchanger* seedExchanger) :
        SessionConfig(sessionDestroyer, messageBlockManager, packetCoder,
            proactor),
        rpcNetwork_(rpcNetwork),
        seedExchanger_(seedExchanger) {}

    bool isValid() const {
        return SessionConfig::isValid() && (rpcNetwork_ != 0) &&
            (seedExchanger_ != 0);
    }

    SessionRpcNetwork* rpcNetwork_;
    PacketSeedExchanger* seedExchanger_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_RPCSESSIONCONFIG_H)
