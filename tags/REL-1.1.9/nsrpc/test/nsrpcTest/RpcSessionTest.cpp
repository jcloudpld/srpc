#include "stdafx.h"
#include "SessionTestFixture.h"
#include "TestClient.h"
#include "TestCachedSessionManager.h"
#include <nsrpc/detail/SessionRpcNetwork.h>
#include <nsrpc/utility/SystemUtil.h>
#include <srpc/detail/ForwardingFunctors.h>
#include <srpc/detail/RpcCommand.h>
#include <srpc/utility/Endian.h>
#include <srpc/Types.h>

using namespace srpc;
using namespace nsrpc;

/**
 * @class TestRpcCommand
 */
class TestRpcCommand : public RpcCommand
{
public:
    TestRpcCommand(const RRpcId& rpcId, UInt32 value) :
        RpcCommand(rpcId),
        marshaler_(value) {}
private:
    virtual ForwardingFunctor& getMarshaler() {
        return marshaler_;
    }
private:
    ForwardingFunctorT<SRPC_TYPELIST_1(RUInt32)> marshaler_;
};

/**
* @class RpcSessionTest
*
* Session Test
*/
class RpcSessionTest : public SessionTestFixture
{
    CPPUNIT_TEST_SUITE(RpcSessionTest);
    CPPUNIT_TEST(testSendRpcCommands);
    CPPUNIT_TEST_SUITE_END();
public:
    virtual void setUp();
    virtual void tearDown();

    virtual SessionManager* createSessionManager() {
        sessionFactory_ = new RpcSessionFactory(proactorTask_->getProactor());
        return new TestCachedSessionManager(*sessionFactory_);
    }
private:
    void testSendRpcCommands();
private:
    nsrpc::RpcSession& getLastSession() {
        return static_cast<TestCachedSessionManager*>(sessionManager_)->
            getLastSession();
    }
private:
    RpcSessionFactory* sessionFactory_;
    TestClient* client_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(RpcSessionTest);

void RpcSessionTest::setUp()
{
    SessionTestFixture::setUp();

    client_ = new TestClient;
    (void)client_->connect(1, getTestAddress());
}


void RpcSessionTest::tearDown()
{
    client_->close();
    delete client_;

    SessionTestFixture::tearDown();

    delete sessionFactory_;
}


void RpcSessionTest::testSendRpcCommands()
{
    const UInt32 valueExpected = 337;

    pause(1);

    const int sendCount = 5;
    const RRpcId rpcId("testRpcId");
    for (int i = 0; i < sendCount; ++i) {
        TestRpcCommand command(rpcId, valueExpected);
        getLastSession().getRpcNetwork()->send(command);
    }

    pause(1);

    for (int i = 0; i < sendCount; ++i) {
        UInt32 received[2] = { 0, };
        CPPUNIT_ASSERT_EQUAL_MESSAGE("received size",
            static_cast<int>(sizeof(received)),
            client_->recvMessage(received, sizeof(received)));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("RpcId",
            static_cast<int>(rpcId.get()),
            static_cast<int>(toRpcByteOrder(received[0])));
        CPPUNIT_ASSERT_EQUAL_MESSAGE("parameter",
            valueExpected,
            toRpcByteOrder(received[1]));
    }
}
