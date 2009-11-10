#ifndef NSRPC_TESTRPCSESSIONMANAGER_H
#define NSRPC_TESTRPCSESSIONMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>
#include <nsrpc/config/Proactor.h>

#if defined(NSRPC_HAS_PROACTOR)

#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/CachedSessionManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/RpcProactorSession.h>
#include <nsrpc/RpcSessionConfig.h>
#include <nsrpc/PacketSeedExchangerFactory.h>

/**
 * @class TestRpcProactorSession
 */
class TestRpcProactorSession : public nsrpc::RpcProactorSession
{
public:
    TestRpcProactorSession(const nsrpc::RpcSessionConfig& config) :
        nsrpc::RpcProactorSession(config) {}

    void sendRpcCommand(srpc::RpcCommand& command) {
        getRpcNetwork().send(command);
    }
private:
    virtual void onDisconnected() {}
};


/**
 * @class RpcProactorSessionFactory
 */
class RpcProactorSessionFactory : public nsrpc::SessionFactory
{
public:
    RpcProactorSessionFactory(NSRPC_Proactor* proactor = 0,
        bool useBitPacking = true) :
        proactor_(proactor),
        useBitPacking_(useBitPacking),
        sessionDestroyer_(0) {
        messageBlockManager_.reset(
            new nsrpc::SynchMessageBlockManager(10, 1024));
    }

    virtual void setSessionDestroyer(
        nsrpc::SessionDestroyer& sessionDestroyer) {
        sessionDestroyer_ = &sessionDestroyer;
    }

    virtual nsrpc::Session* create() const {
        const nsrpc::RpcSessionConfig config(sessionDestroyer_,
            messageBlockManager_.get(), nsrpc::PacketCoderFactory().create(),
            proactor_, nsrpc::SessionCapacity::getNoLimitedCapacity(),
            new nsrpc::SessionRpcNetwork(useBitPacking_),
            nsrpc::PacketSeedExchangerFactory::createForServer());
        return new TestRpcProactorSession(config);
    }
private:
    NSRPC_Proactor* proactor_;
    bool useBitPacking_;
    boost::scoped_ptr<nsrpc::SynchMessageBlockManager> messageBlockManager_;
    nsrpc::SessionDestroyer* sessionDestroyer_;
};


/**
 * @class TestCachedSessionManager
 *
 * 테스트용 CachedSessionManager
 */
class TestCachedSessionManager : public nsrpc::CachedSessionManager
{
public:
    enum {
        poolSize = 2
    };
public:
    TestCachedSessionManager(nsrpc::SessionFactory& sessionFactory) :
        nsrpc::CachedSessionManager("ForTest", poolSize, sessionFactory),
        lastSession_(0) {}

    virtual nsrpc::Session* acquire() {
        lastSession_ = nsrpc::CachedSessionManager::acquire();
        return lastSession_;
    }
public:
    nsrpc::RpcProactorSession& getLastSession() const {
        return static_cast<nsrpc::RpcProactorSession&>(*lastSession_);
    }
private:
    nsrpc::Session* lastSession_;
};

#endif // #if defined(NSRPC_HAS_PROACTOR)

#endif // !defined(NSRPC_TESTRPCSESSIONMANAGER_H)
