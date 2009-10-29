#ifndef NSRPC_TESTPROACTORSESSION_H
#define NSRPC_TESTPROACTORSESSION_H

#include <nsrpc/nsrpc.h>

#if defined(NSRPC_HAS_PROACTOR)

#include <nsrpc/ProactorSession.h>
#include <nsrpc/detail/PacketCoderFactory.h>

/**
 * @class TestProactorSession
 *
 * �׽�Ʈ�� ProactorSession
 */
class TestProactorSession : public nsrpc::ProactorSession
{
public:
    TestProactorSession(const nsrpc::SessionConfig& config) :
        nsrpc::ProactorSession(config),
        arrivedMessageCount_(0),
        acquireSendBlockCallCount_(0) {}

    size_t getArrivedMessageCount() const {
        return arrivedMessageCount_;
    }

    size_t getAcquireSendBlockCallCount() const {
        return acquireSendBlockCallCount_;
    }
public:
    virtual ACE_Message_Block& acquireSendBlock() {
        ++acquireSendBlockCallCount_;
        return nsrpc::ProactorSession::acquireSendBlock();
    }
private:
    virtual bool onMessageArrived(nsrpc::CsMessageType /*messageType*/) {
        ++arrivedMessageCount_;
        return true;
    }
    virtual void onDisconnected() {}
private:
    size_t arrivedMessageCount_;
    size_t acquireSendBlockCallCount_;
};

#endif // #if defined(NSRPC_HAS_PROACTOR)

#endif // !defined(NSRPC_TESTPROACTORSESSION_H)