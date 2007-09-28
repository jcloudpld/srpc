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
        recvSize_(0) {}

    bool isDisconnected() const {
        return disconnected_;
    }

    size_t getRecvSize() const {
        return recvSize_;
    }
private:
    virtual void onConnected() {}
    virtual void onDisconnected() {
        disconnected_ = true;
    }
    virtual void onMessageArrived(nsrpc::CsMessageType /*messageType*/) {
        recvSize_ += acquireRecvBlock().length();
    }
public: // for Test
    virtual ACE_Message_Block& acquireSendBlock() {
        return nsrpc::ClientSession::acquireSendBlock();
    }
    //virtual ACE_Message_Block& acquireRecvBlock();
    //virtual void release(ACE_Message_Block& mblock);
private:
    bool disconnected_;
    size_t recvSize_;
};

#endif // !defined(NSRPC_TESTCLIENTSESSION_H)
