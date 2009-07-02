#include "stdafx.h"
#include "AceTestFixture.h"
#include <nsrpc/sao/Processor.h>
#include <nsrpc/sao/Proxy.h>
#include <nsrpc/sao/detail/RpcServant.h>
#include <nsrpc/sao/detail/RpcMethod.h>
#include <nsrpc/sao/detail/MethodMemoryManager.h>
#include <nsrpc/utility/SystemUtil.h>
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <ace/Message_Block.h>
#include <ace/Thread_Mutex.h>
#pragma warning( pop )

using namespace nsrpc;
using namespace nsrpc::sao;

/**
* @class MockRpcServant
*/
class MockRpcServant : public RpcServant
{
public:
    MockRpcServant() :
        dispatched_(false) {}

    virtual void dispatch(ACE_Message_Block& /*mblock*/) {
        dispatched_ = true;
    }
public:
    bool dispatched_;
};


/**
* @class RpcServiceProcessorTest
*
* ServiceProcessor(Active Object) Å×½ºÆ®
*/
class RpcServiceProcessorTest : public AceTestFixture 
{ 
public:
    virtual void SetUp() {
        AceTestFixture::SetUp();

        serviceProcessor_ = new Processor;

        EXPECT_EQ(true, serviceProcessor_->initialize());
    }

    virtual void TearDown() {
        AceTestFixture::TearDown();

        serviceProcessor_->finalize();
        delete serviceProcessor_;
    }

protected:
    MockRpcServant rpcServant_;
    Processor* serviceProcessor_;
};


TEST_F(RpcServiceProcessorTest, testRpcDispatch)
{
    MethodMemoryManager<RpcMethod, ACE_Thread_Mutex> rpcMethodManager;

    ACE_Message_Block dummy(100);
    dummy.copy("0123456789");

    for (int i = 0; i < 10; ++i) {
        RpcMethod* method = rpcMethodManager.acquire();
        method->setParameter(&rpcServant_, &dummy);
        serviceProcessor_->getProxy().schedule(method);
    }
    nsrpc::pause(50);

    //serviceProcessor_->finalize();
    EXPECT_EQ(true, rpcServant_.dispatched_);
}
