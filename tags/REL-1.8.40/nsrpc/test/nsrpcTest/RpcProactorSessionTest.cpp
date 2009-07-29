#include "stdafx.h"
#include "ProactorSessionTestFixture.h"

#if defined(NSRPC_HAS_PROACTOR)

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
    TestRpcCommand(const RRpcId& rpcId, const RUInt32& value) :
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
* @class RpcProactorSessionTest
*
* RpcProactorSession Test
*/
class RpcProactorSessionTest : public ProactorSessionTestFixture
{
private:
    virtual void SetUp() {
        ProactorSessionTestFixture::SetUp();

        client_ = new TestClient;
        (void)client_->connect(1, getTestAddress());
    }

    virtual void TearDown() {
        client_->close();
        delete client_;

        ProactorSessionTestFixture::TearDown();

        delete sessionFactory_;
    }

private:
    virtual SessionManager* createSessionManager() {
        sessionFactory_ = new RpcProactorSessionFactory(proactorTask_->getProactor());
        return new TestCachedSessionManager(*sessionFactory_);
    }

protected:
    TestRpcProactorSession& getLastSession() {
        return static_cast<TestRpcProactorSession&>(
            static_cast<TestCachedSessionManager*>(sessionManager_)->
                getLastSession());
    }

protected:
    RpcProactorSessionFactory* sessionFactory_;
    TestClient* client_;
};


TEST_F(RpcProactorSessionTest, testSendRpcCommands)
{
    const RUInt32 valueExpected = 337;

    pause(1);

    const int sendCount = 5;
    const RRpcId rpcId("testRpcId");
    for (int i = 0; i < sendCount; ++i) {
        TestRpcCommand command(rpcId, valueExpected);
        getLastSession().sendRpcCommand(command);
    }

    pause(1);

    for (int i = 0; i < sendCount; ++i) {
        UInt32 received[2] = { 0, };
        EXPECT_EQ(sizeof(received),
            size_t(client_->recvMessage(received, sizeof(received))));
        EXPECT_EQ(rpcId.get(), toRpcByteOrder(received[0]));
        EXPECT_EQ(valueExpected.get(), toRpcByteOrder(received[1]));
    }
}

#endif // #if defined(NSRPC_HAS_PROACTOR)
