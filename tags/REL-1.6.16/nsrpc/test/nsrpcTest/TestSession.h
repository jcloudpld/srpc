#ifndef NSRPC_TESTSESSION_H
#define NSRPC_TESTSESSION_H

#include <nsrpc/Session.h>
#include <nsrpc/detail/PacketCoderFactory.h>

/**
 * @class TestSession
 *
 * 테스트용 Session
 */
class TestSession : public nsrpc::Session
{
public:
    TestSession(const nsrpc::SessionConfig& config) :
        nsrpc::Session(config),
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
        return nsrpc::Session::acquireSendBlock();
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

#endif // !defined(NSRPC_TESTSESSION_H)
