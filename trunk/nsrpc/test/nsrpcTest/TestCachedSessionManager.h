#ifndef NSRPC_TESTRPCSESSIONMANAGER_H
#define NSRPC_TESTRPCSESSIONMANAGER_H

#include <nsrpc/nsrpc.h>

#if defined(NSRPC_HAS_PROACTOR)

#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/CachedSessionManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/RpcSession.h>
#include <nsrpc/RpcSessionConfig.h>
#include <nsrpc/PacketSeedExchangerFactory.h>

/**
 * @class TestRpcSession
 */
class TestRpcSession : public nsrpc::RpcSession
{
public:
    TestRpcSession(const nsrpc::RpcSessionConfig& config) :
        nsrpc::RpcSession(config) {}
private:
    virtual void onDisconnected() {}
};


/**
 * @class RpcSessionFactory
 */
class RpcSessionFactory : public nsrpc::SessionFactory
{
public:
    RpcSessionFactory(NSRPC_Proactor* proactor = 0,
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
        return new TestRpcSession(config);
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
    nsrpc::RpcSession& getLastSession() const {
        return static_cast<nsrpc::RpcSession&>(*lastSession_);
    }
private:
    nsrpc::Session* lastSession_;
};

#endif // #if defined(NSRPC_HAS_PROACTOR)

#endif // !defined(NSRPC_TESTRPCSESSIONMANAGER_H)
