#ifndef NSRPC_P2PRELAYTESTFIXTURE_H
#define NSRPC_P2PRELAYTESTFIXTURE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "P2pSessionTestFixture.h"
#include <nsrpc/p2p/server/SimpleRelayService.h>
#include <nsrpc/ReactorTask.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/detail/PacketCoderFactory.h>

using namespace nsrpc;

/**
* @class P2pRelayTestFixture
*
* P2P P2pSession Test
*/
class P2pRelayTestFixture : public P2pSessionTestFixture
{
protected:
    enum { relayServerPort = 1004 };

protected:
    virtual void SetUp() {
        P2pSessionTestFixture::SetUp();

        reactorTask_ = new ReactorTask(true);
        relayService_ = new detail::SimpleRelayService(
            reactorTask_->getReactor(), PacketCoderFactory().createForP2p());
        EXPECT_TRUE(relayService_->open(relayServerPort)) <<
            "open relay server";

        reactorTask_->start(1);

        hostSession_->setRelayServer(
            PeerAddress(ACE_LOCALHOST, relayServerPort));
        for (int i = 0; i < 2; ++i) {
            hostSession_->tick();
        }
    }

    virtual void TearDown() {
        P2pSessionTestFixture::TearDown();

        relayService_->close();

        reactorTask_->stop();
        reactorTask_->wait();

        delete relayService_;
        delete reactorTask_;
    }

protected:
    virtual PeerAddresses getHostAddresses() const {
        PeerAddresses addresses;
        addresses.push_back(
            PeerAddress(ACE_LOCALHOST, ACE_DEFAULT_SERVER_PORT));
        return addresses;
    }

protected:
    ReactorTask* reactorTask_;
    detail::SimpleRelayService* relayService_;
};

#endif // !defined(NSRPC_P2PRELAYTESTFIXTURE_H)
