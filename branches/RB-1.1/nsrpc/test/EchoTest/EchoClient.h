#ifndef ECHOTEST_ECHOCLIENT_H
#define ECHOTEST_ECHOCLIENT_H

#include "Config.h"
#include "EchoInterface.h"
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/RpcSession.h>
#include <nsrpc/PacketSeedExchanger.h>
#include <nsrpc/detail/PacketCoder.h>

/**
 * @class EchoClientSession
 */
class EchoClientSession : public nsrpc::Session
{
public:
    EchoClientSession(srpc::UInt32 echoCount, srpc::UInt32 blockSize,
        nsrpc::SessionDestroyer& sessionDestroyer,
        nsrpc::SynchMessageBlockManager& messageBlockManager,
        NSRPC_Proactor* proactor) :
        nsrpc::Session(sessionDestroyer, nsrpc::PacketCoderFactory().create(),
            messageBlockManager, proactor),
        echoCount_(echoCount),
        blockSize_(blockSize),
        body_(blockSize - getPacketCoder().getHeaderSize(), 'X'),
        echoedCount_(0) {}
private:
    virtual bool onMessageArrived(nsrpc::CsMessageType /*messageType*/) {
        assert(acquireRecvBlock().length() == body_.length());
        assert(echoedCount_ < echoCount_);
        if ((++echoedCount_) >= echoCount_) {
            return false; // or cancel();
        }

        send();
        return true;
    }

    virtual void onDisconnected() {}

    virtual void open(ACE_HANDLE new_handle,
        ACE_Message_Block& message_block) {
        nsrpc::Session::open(new_handle, message_block);
        echoedCount_ = 0;
        send();
    }
private:
    void send() {
        ACE_Message_Block& mblock = acquireSendBlock();
        const void* bodyPtr = body_.data();
        mblock.copy(static_cast<const char*>(bodyPtr), body_.length());
        sendMessage(mblock);
    }
private:
    const srpc::UInt32 echoCount_;
    const srpc::UInt32 blockSize_;
    const srpc::String body_;
    srpc::UInt32 echoedCount_;
};


/**
* @class EchoClientRpcSession
*/
class EchoClientRpcSession :
    public nsrpc::RpcSession, public Echo
{
public:
    EchoClientRpcSession(srpc::UInt32 echoCount, srpc::UInt32 blockSize,
        nsrpc::SessionDestroyer& sessionDestroyer,
        nsrpc::SynchMessageBlockManager& messageBlockManager,
        NSRPC_Proactor* proactor, nsrpc::SessionRpcNetwork* rpcNetwork) :
        nsrpc::RpcSession(sessionDestroyer,
            nsrpc::PacketCoderFactory().create(), messageBlockManager,
            proactor, rpcNetwork,
            nsrpc::PacketSeedExchanger::createForClient()),
        echoCount_(echoCount),
        blockSize_(blockSize),
        body_(blockSize - getPacketCoder().getHeaderSize(), 'R') {}

    DECLARE_SRPC_METHOD_1(echo, srpc::RString, data);
    DECLARE_SRPC_METHOD_1(onEcho, srpc::RString, data);
private:
    virtual void open(ACE_HANDLE new_handle,
        ACE_Message_Block& message_block) {
        nsrpc::RpcSession::open(new_handle, message_block);
        echoedCount_ = 0;
        echo(body_);
    }

    virtual void onDisconnected() {}
private:
    srpc::UInt32 echoCount_;
    srpc::UInt32 blockSize_;
    const srpc::String body_;
    srpc::UInt32 echoedCount_;
};


/**
* @class ClientSessionFactory
*/
class ClientSessionFactory : public nsrpc::SessionFactory
{
public:
    ClientSessionFactory(const Config& config,
        NSRPC_Proactor* proactor, bool useBitPacking) :
        nsrpc::SessionFactory(proactor, useBitPacking),
        config_(config),
        messageBlockManager_(100, 1024) {}

    virtual nsrpc::Session* create(
        nsrpc::SessionDestroyer& sessionDestroyer) const {
        nsrpc::SynchMessageBlockManager& messageBlockManager =
            const_cast<ClientSessionFactory*>(this)->messageBlockManager_;
        if (config_.useRpc()) {
            return new EchoClientRpcSession(config_.getEchoCount(),
                config_.getBlockSize(), sessionDestroyer,
                messageBlockManager, getProactor(),
                new nsrpc::SessionRpcNetwork(useBitPacking()));
        }
        return new EchoClientSession(config_.getEchoCount(),
            config_.getBlockSize(), sessionDestroyer, messageBlockManager,
            getProactor());
    }
private:
    const Config& config_;
    nsrpc::SynchMessageBlockManager messageBlockManager_;
};

#endif // ECHOTEST_ECHOCLIENT_H
