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
    TestSession(nsrpc::SessionDestroyer& sessionDestroyer,
        nsrpc::PacketCoder* packetCoder,
        nsrpc::SynchMessageBlockManager& messageBlockManager,
        NSRPC_Proactor* proactor) :
        nsrpc::Session(sessionDestroyer, packetCoder, messageBlockManager,
            proactor),
        arrivedMessageSize_(0),
        acquireSendBlockCallCount_(0),
        acquireRecvBlockCallCount_(0) {}

    size_t getArrivedMessageSize() const {
        return arrivedMessageSize_;
    }

    size_t getAcquireSendBlockCallCount() const {
        return acquireSendBlockCallCount_;
    }
    size_t getAcquireRecvBlockCallCount() const {
        return acquireRecvBlockCallCount_;
    }
public:
    virtual ACE_Message_Block& acquireSendBlock() {
        ++acquireSendBlockCallCount_;
        return nsrpc::Session::acquireSendBlock();
    }
    virtual ACE_Message_Block& acquireRecvBlock() {
        ++acquireRecvBlockCallCount_;
        return nsrpc::Session::acquireRecvBlock();
    }
private:
    virtual bool onMessageArrived(nsrpc::CsMessageType /*messageType*/) {
        arrivedMessageSize_ += acquireRecvBlock().length();
        return true;
    }
    virtual void onDisconnected() {}
private:
    size_t arrivedMessageSize_;
    size_t acquireSendBlockCallCount_;
    size_t acquireRecvBlockCallCount_;
};

#endif // !defined(NSRPC_TESTSESSION_H)
