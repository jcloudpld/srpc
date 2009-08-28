#ifndef NSRPC_RPCSESSIONCONFIG_H
#define NSRPC_RPCSESSIONCONFIG_H

#ifdef _MSC_VER
#  pragma once
#endif

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
        const SessionCapacity& capacity,
        SessionRpcNetwork* rpcNetwork, PacketSeedExchanger* seedExchanger,
        bool shouldUseUtf8ForString = true) :
        SessionConfig(sessionDestroyer, messageBlockManager, packetCoder,
            proactor, capacity),
        rpcNetwork_(rpcNetwork),
        seedExchanger_(seedExchanger),
        shouldUseUtf8ForString_(shouldUseUtf8ForString) {}

    bool isValid() const {
        return SessionConfig::isValid() && (rpcNetwork_ != 0) &&
            (seedExchanger_ != 0);
    }

    SessionRpcNetwork* rpcNetwork_;
    PacketSeedExchanger* seedExchanger_;
    /// 문자열 타입의 경우 호환성을 위해 UTF-8을 이용할 것인가?
    bool shouldUseUtf8ForString_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_RPCSESSIONCONFIG_H)
