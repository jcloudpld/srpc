#ifndef NSRPC_TESTRPCSESSIONMANAGER_H
#define NSRPC_TESTRPCSESSIONMANAGER_H

#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/CachedSessionManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/RpcSession.h>
#include <nsrpc/PacketSeedExchanger.h>

/**
 * @class TestRpcSession
 */
class TestRpcSession : public nsrpc::RpcSession
{
public:
    TestRpcSession(nsrpc::SessionDestroyer& sessionDestroyer,
        nsrpc::PacketCoder* packetCoder,
        nsrpc::SynchMessageBlockManager& messageBlockManager,
        NSRPC_Proactor* proactor, nsrpc::SessionRpcNetwork* rpcNetwork) :
        nsrpc::RpcSession(sessionDestroyer,
            packetCoder, messageBlockManager, proactor, rpcNetwork,
            nsrpc::PacketSeedExchanger::createForServer()) {}
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
        nsrpc::SessionFactory(proactor, useBitPacking) {
        messageBlockManager_.reset(
            new nsrpc::SynchMessageBlockManager(10, 1024));
    }

    virtual nsrpc::Session* create(
        nsrpc::SessionDestroyer& sessionDestroyer) const {
        return new TestRpcSession(sessionDestroyer,
            nsrpc::PacketCoderFactory().create(),
            *messageBlockManager_, getProactor(),
            new nsrpc::SessionRpcNetwork(useBitPacking()));
    }
private:
    boost::scoped_ptr<nsrpc::SynchMessageBlockManager> messageBlockManager_;
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

#endif // !defined(NSRPC_TESTRPCSESSIONMANAGER_H)
