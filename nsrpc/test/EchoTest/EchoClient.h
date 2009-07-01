#ifndef ECHOTEST_ECHOCLIENT_H
#define ECHOTEST_ECHOCLIENT_H

#include <nsrpc/nsrpc.h>

#if defined(NSRPC_HAS_PROACTOR)

#include "Config.h"
#include "EchoInterface.h"
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/RpcSession.h>
#include <nsrpc/RpcSessionConfig.h>
#include <nsrpc/PacketSeedExchangerFactory.h>
#include <nsrpc/detail/PacketCoder.h>

/**
 * @class EchoClientSession
 */
class EchoClientSession : public nsrpc::Session
{
public:
    EchoClientSession(srpc::UInt32 echoCount, srpc::UInt32 blockSize,
        const nsrpc::SessionConfig& config) :
        nsrpc::Session(config),
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
        const nsrpc::RpcSessionConfig& config) :
        nsrpc::RpcSession(config),
        echoCount_(echoCount),
        blockSize_(blockSize),
        body_(blockSize - getPacketCoder().getHeaderSize(), 'R') {}

    OVERRIDE_SRPC_METHOD_1(echo, srpc::RString, data);
    OVERRIDE_SRPC_METHOD_1(onEcho, srpc::RString, data);
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
            const_cast<ClientSessionFactory*>(this)->messageBlockManager_;
        if (config_.useRpc()) {
            return new EchoClientRpcSession(config_.getEchoCount(),
                config_.getBlockSize(),
                nsrpc::RpcSessionConfig(sessionDestroyer_,
                    &messageBlockManager,
                    nsrpc::PacketCoderFactory().create(), proactor_,
                    nsrpc::SessionCapacity::getNoLimitedCapacity(),
                    new nsrpc::SessionRpcNetwork(useBitPacking_),
                    nsrpc::PacketSeedExchangerFactory::createForClient()));
        }
        return new EchoClientSession(config_.getEchoCount(),
            config_.getBlockSize(),
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

#endif // ECHOTEST_ECHOCLIENT_H
