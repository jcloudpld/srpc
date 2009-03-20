#include "stdafx.h"
#include <nsrpc/utility/MemoryPool.h>

using namespace nsrpc;

/**
* @class MemoryPoolTest
*
* MemoryPool Test
*/
class MemoryPoolTest : public testing::Test
{
private:
    virtual void SetUp() {
        pool_ = new TestingPool(poolSize, allocator_);
        pool_->initialize();
    }

    virtual void TearDown() {
        delete pool_;
    }

protected:
    enum {
        memorySize = 128,
        poolSize = 2
    };

    MemoryAllocator<memorySize> allocator_;
    typedef MemoryPool<memorySize> TestingPool;
    TestingPool* pool_;
};


TEST_F(MemoryPoolTest, testInitialize)
{
    EXPECT_EQ(0, pool_->getActiveResourceCount());
    EXPECT_EQ(poolSize, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testAcquire)
{
    void* resource = pool_->acquire();
    EXPECT_TRUE(0 != resource);

    EXPECT_EQ(1, pool_->getActiveResourceCount());
    EXPECT_EQ(poolSize - 1, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testRelease)
{
    void* resource = pool_->acquire();

    pool_->release(resource);
    EXPECT_EQ(0, pool_->getActiveResourceCount());
    EXPECT_EQ(poolSize, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testMassAcquireAndRelease)
{
    const int acquireCount = 10;
    void* resources[acquireCount];

    for (int i = 0; i < acquireCount; ++i) {
        resources[i] = pool_->acquire();
    }

    EXPECT_EQ(acquireCount, pool_->getActiveResourceCount());
    EXPECT_EQ(0, pool_->getInactiveResourceCount());

    for (int i = acquireCount - 1; i >= 0; --i) {
        pool_->release(resources[i]);
    }

    EXPECT_EQ(0, pool_->getActiveResourceCount());
    EXPECT_EQ(10, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testDestroy)
{
    for (int i = 0; i < 10; ++i) {
        (void)pool_->acquire();
    }

    pool_->destroy();

    EXPECT_EQ(0, pool_->getActiveResourceCount());
    EXPECT_EQ(0, pool_->getInactiveResourceCount());
}
