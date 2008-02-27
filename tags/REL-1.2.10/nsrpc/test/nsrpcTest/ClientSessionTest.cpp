#include "stdafx.h"
#include "ClientSessionTestFixture.h"
#include "TestClient.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace srpc;
using namespace nsrpc;

/**
* @class ClientSessionTest
*
* Session Test
*/
class ClientSessionTest : public ClientSessionTestFixture
{
    CPPUNIT_TEST_SUITE(ClientSessionTest);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST(testDisconnected);
    CPPUNIT_TEST(testSendPackets);
    CPPUNIT_TEST(testRecvPackets);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();
private:
    void testConnect();
    void testDisconnected();
    void testSendPackets();
    void testRecvPackets();
private:
    TestSessionManager* getSessionManager() {
        return static_cast<TestSessionManager*>(sessionManager_);
    }

    TestSession& getLastServerSession() {
        return getSessionManager()->getSession();
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION(ClientSessionTest);

void ClientSessionTest::setUp()
{
    ClientSessionTestFixture::setUp();

    clientSession_->connect(getTestAddress().get_host_addr(),
        getTestAddress().get_port_number(), 1);

    pause(1);
}


void ClientSessionTest::tearDown()
{
    clientSession_->disconnect();

    ClientSessionTestFixture::tearDown();
}


void ClientSessionTest::testConnect()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("one session",
        1, getSessionManager()->getSessionCount());
}


void ClientSessionTest::testDisconnected()
{
    getLastServerSession().disconnect();

    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("disconnected",
        false, clientSession_->isConnected());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("disconnected",
        true, clientSession_->isDisconnected());
    //CPPUNIT_ASSERT_EQUAL_MESSAGE("no session",
    //    0, getSessionManager()->getSessionCount());
}


void ClientSessionTest::testSendPackets()
{
    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        ACE_Message_Block& mblock = clientSession_->acquireSendBlock();
        const void* bodyPtr = body;
        mblock.copy(static_cast<const char*>(bodyPtr), bodySize);
        clientSession_->sendMessage(mblock, mtGeneral);
    }

    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("received bytes",
        static_cast<int>(
            packetCoder_->getHeaderSize() + bodySize) * sendCount,
        static_cast<int>(getLastServerSession().getStats().recvBytes_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("received bytes",
        static_cast<int>(bodySize) * sendCount,
        static_cast<int>(getLastServerSession().getArrivedMessageSize()));
}


void ClientSessionTest::testRecvPackets()
{
    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        ACE_Message_Block& mblock = getLastServerSession().acquireSendBlock();
        const void* bodyPtr = body;
        mblock.copy(static_cast<const char*>(bodyPtr), bodySize);
        getLastServerSession().sendMessage(mblock);
    }

    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("sent bytes",
        static_cast<int>(
            packetCoder_->getHeaderSize() + bodySize) * sendCount,
        static_cast<int>(getLastServerSession().getStats().sentBytes_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("recv bytes",
        static_cast<int>(bodySize * sendCount),
        static_cast<int>(clientSession_->getRecvSize()));
}
