#ifndef NSRPC_SESSIONTESTFIXTURE_H
#define NSRPC_SESSIONTESTFIXTURE_H

#include "AceTestFixture.h"
#include "TestSessionManager.h"
#include <nsrpc/SessionAcceptor.h>
#include <nsrpc/ProactorFactory.h>
#include <nsrpc/ProactorTask.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/detail/PacketCoder.h>

/**
* @class SessionTestFixture
*
* Session TestFixture
*/
class SessionTestFixture : public AceTestFixture
{
public:
    virtual void setUp() {
        AceTestFixture::setUp();

        nsrpc::disableSignals(ACE_SIGRTMIN, ACE_SIGRTMAX);
        nsrpc::disableSignals(SIGPIPE, SIGPIPE);
        nsrpc::disableSignals(SIGIO, SIGIO);

#if defined(ACE_WIN32)
        proactorTask_ = new nsrpc::ProactorTask(
            nsrpc::ProactorFactory::create(nsrpc::ptWin32), true);
#elif defined(ACE_HAS_LINUX_EPOLL)
        proactorTask_ = new nsrpc::ProactorTask(
            nsrpc::ProactorFactory::create(nsrpc::ptEpoll), true);
#else
        proactorTask_ = new nsrpc::ProactorTask(
            nsrpc::ProactorFactory::create(nsrpc::ptSelect), true);
#endif
        proactorTask_->start(1);

        sessionManager_ = createSessionManager();
        sessionManager_->initialize();

        acceptor_ = new nsrpc::SessionAcceptor(*sessionManager_);
        (void)acceptor_->open(
            ACE_INET_Addr(getTestAddress().get_port_number()),
            proactorTask_->getProactor());
        acceptor_->start();

        packetCoder_ = nsrpc::PacketCoderFactory().create();
    }

    virtual void tearDown() {
        if (acceptor_ != 0) {
            acceptor_->close();
            acceptor_->wait();
        }

        sessionManager_->cancel();
        sessionManager_->wait();

        proactorTask_->stop();
        proactorTask_->wait();

        delete acceptor_;
        delete sessionManager_;
        delete proactorTask_;

        delete packetCoder_;

        AceTestFixture::tearDown();
    }
private:
    virtual nsrpc::SessionManager* createSessionManager() {
        return new TestSessionManager(proactorTask_->getProactor());
    }
protected:
    ACE_INET_Addr getTestAddress() const {
        return ACE_INET_Addr(ACE_DEFAULT_SERVER_PORT, ACE_LOCALHOST);
    }
protected:
    nsrpc::ProactorTask* proactorTask_;
    nsrpc::SessionManager* sessionManager_;
    nsrpc::SessionAcceptor* acceptor_;
    nsrpc::PacketCoder* packetCoder_;
};

#endif // !defined(NSRPC_SESSIONTESTFIXTURE_H)