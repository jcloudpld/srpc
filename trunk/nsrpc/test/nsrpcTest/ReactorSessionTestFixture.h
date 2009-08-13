#ifndef NSRPC_REACTORSESSIONTESTFIXTURE_H
#define NSRPC_REACTORSESSIONTESTFIXTURE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>
#include <nsrpc/config/Proactor.h>

#if defined(NSRPC_HAS_PROACTOR)

#include "ProactorSessionTestFixture.h"
#include "TestReactorSession.h"
#include <nsrpc/ReactorTask.h>
#include <nsrpc/detail/PacketCoderFactory.h>
#include <nsrpc/detail/PacketCoder.h>

/**
* @class ReactorSessionTestFixture
*
* ReactorSession TestFixture
*/
class ReactorSessionTestFixture : public ProactorSessionTestFixture
{
public:
    virtual void SetUp() {
        ProactorSessionTestFixture::SetUp();

        reactorTask_ = new nsrpc::ReactorTask(true);
        reactorTask_->start(1);

        clientSession_ = new TestReactorSession(reactorTask_->getReactor());

        packetCoder_ = nsrpc::PacketCoderFactory().create();
    }

    virtual void TearDown() {
        delete clientSession_;

        reactorTask_->stop();
        reactorTask_->wait();
        delete reactorTask_;

        delete packetCoder_;

        ProactorSessionTestFixture::TearDown();
    }
protected:
    nsrpc::ReactorTask* reactorTask_;
    TestReactorSession* clientSession_;
    nsrpc::PacketCoder* packetCoder_;
};

#endif // #if defined(NSRPC_HAS_PROACTOR)

#endif // !defined(NSRPC_REACTORSESSIONTESTFIXTURE_H)
