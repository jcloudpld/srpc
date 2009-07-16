#ifndef ECHOTEST_ECHOSERVER_H
#define ECHOTEST_ECHOSERVER_H

#include <nsrpc/nsrpc.h>

#if defined(NSRPC_HAS_PROACTOR)

#include "Config.h"
#include "EchoInterface.h"
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/RpcProactorSession.h>
#include <nsrpc/RpcSessionConfig.h>
#include <nsrpc/PacketSeedExchangerFactory.h>
#include <srpc/RpcForwarder.h>
#include <srpc/RpcReceiver.h>

/**
 * @class EchoServerSession
 */
class EchoServerSession : public nsrpc::ProactorSession
{
public:
    EchoServerSession(const nsrpc::SessionConfig& config) :
        nsrpc::ProactorSession(config) {}
private:
    virtual bool onMessageArrived(nsrpc::CsMessageType /*messageType*/) {
        echo();
        return true;
    }
    virtual void onDisconnected() {}
private:
    void echo() {
        ACE_Message_Block& sblock = acquireSendBlock();
        const ACE_Message_Block& rblock = acquireRecvBlock();
        sblock.copy(rblock.rd_ptr(), rblock.length());
        sendMessage(sblock);
    }
};


/**
* @class EchoServerRpcSession
*/
class EchoServerRpcSession :
    public nsrpc::RpcProactorSession, public Echo,
    protected srpc::RpcForwarder, protected srpc::RpcReceiver
{
    DECLARE_SRPC_EVENT_DISPATCHER(EchoServerRpcSession);
public:
    EchoServerRpcSession(const nsrpc::RpcSessionConfig& config);

    OVERRIDE_SRPC_METHOD_1(echo, srpc::RString, data);
    OVERRIDE_SRPC_METHOD_1(onEcho, srpc::RString, data);
private:
    virtual void onDisconnected() {}
};


/**
* @class ServerSessionFactory
*/
class ServerSessionFactory : public nsrpc::SessionFactory
{
public:
    ServerSessionFactory(const Config& config,
        NSRPC_Proactor* proactor, bool useBitPacking) :
        config_(config),
        proactor_(proactor),
        useBitPacking_(useBitPacking),
        messageBlockManager_(100, 1024),
        sessionDestroyer_(0) {}

    virtual void setSessionDestroyer(
        nsrpc::SessionDestroyer& sessionDestroyer) {
        sessionDestroyer_ = &sessionDestroyer;
    }

    virtual nsrpc::Session* create() const {
        nsrpc::SynchMessageBlockManager& messageBlockManager =
            const_cast<ServerSessionFactory*>(this)->messageBlockManager_;
        if (config_.useRpc()) {
            return new EchoServerRpcSession(
                nsrpc::RpcSessionConfig(sessionDestroyer_,
                    &messageBlockManager,
                    nsrpc::PacketCoderFactory().create(), proactor_,
                    nsrpc::SessionCapacity::getNoLimitedCapacity(),
                    new nsrpc::SessionRpcNetwork(useBitPacking_),
                    nsrpc::PacketSeedExchangerFactory::createForServer()));
        }
        return new EchoServerSession(
            nsrpc::SessionConfig(sessionDestroyer_, &messageBlockManager,
                nsrpc::PacketCoderFactory().create(), proactor_,
                nsrpc::SessionCapacity::getNoLimitedCapacity()));
    }
private:
    const Config& config_;
    NSRPC_Proactor* proactor_;
    bool useBitPacking_;
    nsrpc::SynchMessageBlockManager messageBlockManager_;
    nsrpc::SessionDestroyer* sessionDestroyer_;
};

#endif // #if defined(NSRPC_HAS_PROACTOR)

#endif // ECHOTEST_ECHOSERVER_H
