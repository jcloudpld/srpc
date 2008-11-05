#include "stdafx.h"
#include "SessionTestFixture.h"
#include "TestClient.h"
#include <nsrpc/utility/SystemUtil.h>

using namespace srpc;
using namespace nsrpc;

/**
* @class SessionTest
*
* Session Test
*/
class SessionTest : public SessionTestFixture
{
    CPPUNIT_TEST_SUITE(SessionTest);
    CPPUNIT_TEST(testRecvPackets);
    CPPUNIT_TEST(testSendPackets);
    CPPUNIT_TEST(testConnect);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();
private:
    void testRecvPackets();
    void testSendPackets();
    void testConnect();
private:
    TestSession& getLastSession() {
        return static_cast<TestSessionManager*>(sessionManager_)->getSession();
    }
private:
    TestClient* client_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SessionTest);

void SessionTest::setUp()
{
    SessionTestFixture::setUp();

    client_ = new TestClient;
    (void)client_->connect(1, getTestAddress());
}


void SessionTest::tearDown()
{
    client_->close();
    delete client_;

    SessionTestFixture::tearDown();
}


void SessionTest::testRecvPackets()
{
    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        client_->sendMessage(body, bodySize);
    }

    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("received bytes",
        static_cast<int>(
            packetCoder_->getHeaderSize() + bodySize) * sendCount,
        static_cast<int>(getLastSession().getStats().recvBytes_));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("received count",
        sendCount,
        static_cast<int>(getLastSession().getArrivedMessageCount()));

    // 아래 테스트는 큰 의미가 없음
    CPPUNIT_ASSERT_EQUAL_MESSAGE("acquireSendBlock call count",
        0,
        static_cast<int>(getLastSession().getAcquireSendBlockCallCount()));
}


void SessionTest::testSendPackets()
{
    pause(1);

    const int sendCount = 5;
    const UInt8 body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;

    for (int i = 0; i < sendCount; ++i) {
        ACE_Message_Block& mblock = getLastSession().acquireSendBlock();
        const void* bodyPtr = body;
        mblock.copy(static_cast<const char*>(bodyPtr), bodySize);
        getLastSession().sendMessage(mblock);
    }

    getLastSession().disconnectGracefully();

    pause(1);

    for (int i = 0; i < sendCount; ++i) {
        UInt8 received[bodySize];
        CPPUNIT_ASSERT_EQUAL_MESSAGE("received size",
            static_cast<int>(bodySize),
            client_->recvMessage(received, bodySize));
        CPPUNIT_ASSERT_MESSAGE("cmp",
            memcmp(body, received, bodySize) == 0);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("sent bytes",
        static_cast<int>(
            packetCoder_->getHeaderSize() + bodySize) * sendCount,
        static_cast<int>(
            getLastSession().getStats().sentBytes_));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("acquireSendBlock call count",
        sendCount,
        static_cast<int>(getLastSession().getAcquireSendBlockCallCount()));
}


void SessionTest::testConnect()
{
    TestSession* session =
        static_cast<TestSession*>(sessionManager_->acquire());

    CPPUNIT_ASSERT_EQUAL_MESSAGE("connect",
        true,
        session->connect(getTestAddress().get_host_name(),
            getTestAddress().get_port_number(), 10));

    pause(1);

    const char body[] = "1234567890";
    const UInt16 bodySize = (sizeof(body) / sizeof(body[0])) - 1;
    ACE_Message_Block& mblock = getLastSession().acquireSendBlock();
    mblock.copy(body, bodySize);
    getLastSession().sendMessage(mblock);

    pause(1);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("received count",
        1,
        static_cast<int>(session->getArrivedMessageCount()));

    sessionManager_->release(session);
}
