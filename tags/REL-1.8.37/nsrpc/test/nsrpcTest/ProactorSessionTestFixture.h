#ifndef NSRPC_PROACTORSESSIONTESTFIXTURE_H
#define NSRPC_PROACTORSESSIONTESTFIXTURE_H

#include <nsrpc/nsrpc.h>
#include <nsrpc/config/Proactor.h>

#if defined(NSRPC_HAS_PROACTOR)

#include "AceTestFixture.h"
#include "TestProactorSessionManager.h"
#include <nsrpc/ProactorSessionAcceptor.h>
#include <nsrpc/ProactorFactory.h>
#include <nsrpc/ProactorTask.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/detail/PacketCoder.h>

/**
* @class ProactorSessionTestFixture
*
* Session TestFixture
*/
class ProactorSessionTestFixture : public AceTestFixture
{
protected:
    virtual void SetUp() {
        AceTestFixture::SetUp();

        nsrpc::disableSignals(ACE_SIGRTMIN, ACE_SIGRTMAX);
        nsrpc::disableSignals(SIGPIPE, SIGPIPE);
        nsrpc::disableSignals(SIGIO, SIGIO);

        NSRPC_Proactor* proactor = 0;

#if defined (ACE_WIN32)
        proactor = nsrpc::ProactorFactory::create(nsrpc::ptWin32);
#elif defined (NSRPC_USE_TPROACTOR)
# if defined (ACE_HAS_LINUX_EPOLL)
        proactor = nsrpc::ProactorFactory::create(nsrpc::ptEpoll);
# else
        proactor = nsrpc::ProactorFactory::create(nsrpc::ptSelect);
# endif
#else
        proactor = nsrpc::ProactorFactory::create(nsrpc::ptAny);
#endif

        proactorTask_ = new nsrpc::ProactorTask(proactor, true);
        proactorTask_->start(1);

        sessionManager_ = createSessionManager();
        sessionManager_->initialize();

        acceptor_ = new nsrpc::ProactorSessionAcceptor(*sessionManager_);
        (void)acceptor_->open(
            ACE_INET_Addr(getTestAddress().get_port_number()),
            proactorTask_->getProactor());

        packetCoder_ = nsrpc::PacketCoderFactory().create();
    }

    virtual void TearDown() {
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

        AceTestFixture::TearDown();
    }

private:
    virtual nsrpc::SessionManager* createSessionManager() {
        return new TestProactorSessionManager(proactorTask_->getProactor());
    }

protected:
    ACE_INET_Addr getTestAddress() const {
        return ACE_INET_Addr(ACE_DEFAULT_SERVER_PORT, ACE_LOCALHOST);
    }

protected:
    nsrpc::ProactorTask* proactorTask_;
    nsrpc::SessionManager* sessionManager_;
    nsrpc::ProactorSessionAcceptor* acceptor_;
    nsrpc::PacketCoder* packetCoder_;
};

#endif // #if defined(NSRPC_HAS_PROACTOR)

#endif // !defined(NSRPC_PROACTORSESSIONTESTFIXTURE_H)
