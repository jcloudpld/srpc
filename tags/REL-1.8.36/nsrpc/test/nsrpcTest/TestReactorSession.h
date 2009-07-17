#ifndef NSRPC_TESTREACTORSESSION_H
#define NSRPC_TESTREACTORSESSION_H

#include <nsrpc/ReactorSession.h>

/**
 * @class TestReactorSession
 * 테스트용 ReactorSession
 */
class TestReactorSession : public nsrpc::ReactorSession
{
public:
    TestReactorSession(ACE_Reactor* reactor) :
        nsrpc::ReactorSession(reactor),
        disconnected_(false),
        messageCount_(0) {}

    bool isDisconnected() const {
        return disconnected_;
    }

    size_t getMessageCount() const {
        return messageCount_;
    }
private:
    virtual void onConnected() {}
    virtual void onDisconnected() {
        disconnected_ = true;
    }
    virtual void onMessageArrived(nsrpc::CsMessageType /*messageType*/) {
        ++messageCount_;
    }
public: // for Test
    virtual ACE_Message_Block& acquireSendBlock() {
        return nsrpc::ReactorSession::acquireSendBlock();
    }
    //virtual ACE_Message_Block& acquireRecvBlock();
    //virtual void release(ACE_Message_Block& mblock);
private:
    bool disconnected_;
    size_t messageCount_;
};

#endif // !defined(NSRPC_TESTREACTORSESSION_H)
