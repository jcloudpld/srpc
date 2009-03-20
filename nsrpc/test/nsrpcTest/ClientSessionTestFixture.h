#ifndef NSRPC_CLIENTSESSIONTESTFIXTURE_H
#define NSRPC_CLIENTSESSIONTESTFIXTURE_H

#include "SessionTestFixture.h"
#include "TestClientSession.h"
#include <nsrpc/ReactorTask.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/detail/PacketCoder.h>

/**
* @class ClientSessionTestFixture
*
* ClientSession TestFixture
*/
class ClientSessionTestFixture : public SessionTestFixture
{
public:
    virtual void SetUp() {
        SessionTestFixture::SetUp();

        reactorTask_ = new nsrpc::ReactorTask(true);
        reactorTask_->start(1);

        clientSession_ = new TestClientSession(reactorTask_->getReactor());

        packetCoder_ = nsrpc::PacketCoderFactory().create();
    }

    virtual void TearDown() {
        delete clientSession_;

        reactorTask_->stop();
        reactorTask_->wait();
        delete reactorTask_;

        delete packetCoder_;

        SessionTestFixture::TearDown();
    }
protected:
    nsrpc::ReactorTask* reactorTask_;
    TestClientSession* clientSession_;
    nsrpc::PacketCoder* packetCoder_;
};

#endif // !defined(NSRPC_CLIENTSESSIONTESTFIXTURE_H)
