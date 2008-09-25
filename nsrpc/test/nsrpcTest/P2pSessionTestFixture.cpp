#include "stdafx.h"
#include "P2pSessionTestFixture.h"
#include <nsrpc/p2p/PeerHint.h>

// = TestRpcPlugIn

IMPLEMENT_SRPC_P2P_METHOD_1(TestRpcPlugIn, hello, srpc::RShortString, world,
    srpc::ptReliable)
{
    const PeerHint& hint = toPeerHint(rpcHint);

    lastPeerId_ = hint.peerId_;
    lastGroupId_ = hint.groupId_;
    lastWorld_ = world;
}

// = P2pSessionTestFixture

void P2pSessionTestFixture::setUp()
{
    AceTestFixture::setUp();

    hostSession_ = P2pSessionFactory::create(1, hostEventHandler_);
    hostRpcPlugIn_ = new TestRpcPlugIn;

    PlugInPtr plugIn(hostRpcPlugIn_);
    hostSession_->attach(plugIn);

    openHost();
}


void P2pSessionTestFixture::tearDown()
{
    delete hostSession_;
    AceTestFixture::tearDown();
}


PeerAddresses P2pSessionTestFixture::getHostAddresses() const
{
    PeerAddresses addresses;
    addresses.push_back(
        PeerAddress(ACE_LOCALHOST, ACE_DEFAULT_SERVER_PORT));
    return addresses;
}


void P2pSessionTestFixture::openHost(const srpc::String& password,
    size_t maxPeers, bool hostMigration)
{
    CPPUNIT_ASSERT_MESSAGE("host open",
        hostSession_->open(ACE_DEFAULT_SERVER_PORT, password));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("no host",
        false, hostSession_->isHost());
    hostSession_->host(maxPeers, hostMigration);
}
