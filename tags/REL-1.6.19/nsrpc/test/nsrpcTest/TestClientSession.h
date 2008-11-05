#ifndef NSRPC_TESTCLIENTSESSION_H
#define NSRPC_TESTCLIENTSESSION_H

#include <nsrpc/detail/ClientSession.h>

/**
 * @class TestClientSession
 * 테스트용 ClientSession
 */
class TestClientSession : public nsrpc::ClientSession
{
public:
    TestClientSession(ACE_Reactor* reactor) :
        nsrpc::ClientSession(reactor),
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
        return nsrpc::ClientSession::acquireSendBlock();
    }
    //virtual ACE_Message_Block& acquireRecvBlock();
    //virtual void release(ACE_Message_Block& mblock);
private:
    bool disconnected_;
    size_t messageCount_;
};

#endif // !defined(NSRPC_TESTCLIENTSESSION_H)
