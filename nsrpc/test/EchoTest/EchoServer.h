#ifndef ECHOTEST_ECHOSERVER_H
#define ECHOTEST_ECHOSERVER_H

#include "Config.h"
#include "EchoInterface.h"
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/RpcSession.h>
#include <nsrpc/PacketSeedExchanger.h>

/**
 * @class EchoServerSession
 */
class EchoServerSession : public nsrpc::Session
{
public:
    EchoServerSession(nsrpc::SessionDestroyer& sessionDestroyer,
        nsrpc::SynchMessageBlockManager& messageBlockManager,
        NSRPC_Proactor* proactor) :
        nsrpc::Session(sessionDestroyer, nsrpc::PacketCoderFactory().create(),
            messageBlockManager, proactor) {}
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
    public nsrpc::RpcSession, public Echo
{
public:
    EchoServerRpcSession(nsrpc::SessionDestroyer& sessionDestroyer,
        nsrpc::SynchMessageBlockManager& messageBlockManager,
        NSRPC_Proactor* proactor, nsrpc::SessionRpcNetwork* rpcNetwork) :
        nsrpc::RpcSession(sessionDestroyer,
            nsrpc::PacketCoderFactory().create(), messageBlockManager,
            proactor, rpcNetwork,
            nsrpc::PacketSeedExchanger::createForServer()) {}

    DECLARE_SRPC_METHOD_1(echo, srpc::RString, data);
    DECLARE_SRPC_METHOD_1(onEcho, srpc::RString, data);
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
        nsrpc::SessionFactory(proactor, useBitPacking),
        config_(config),
        messageBlockManager_(100, 1024) {}

    virtual nsrpc::Session* create(
        nsrpc::SessionDestroyer& sessionDestroyer) const {
        nsrpc::SynchMessageBlockManager& messageBlockManager =
            const_cast<ServerSessionFactory*>(this)->messageBlockManager_;
        if (config_.useRpc()) {
            return new EchoServerRpcSession(sessionDestroyer,
                messageBlockManager, getProactor(),
                new nsrpc::SessionRpcNetwork(useBitPacking()));
        }
        return new EchoServerSession(sessionDestroyer, messageBlockManager,
            getProactor());
    }
private:
    const Config& config_;
    nsrpc::SynchMessageBlockManager messageBlockManager_;
};

#endif // ECHOTEST_ECHOSERVER_H

