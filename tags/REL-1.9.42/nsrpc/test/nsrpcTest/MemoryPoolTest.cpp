#include "stdafx.h"
#include <nsrpc/utility/MemoryPool.h>

using namespace nsrpc;

static const size_t memorySize = 128;
static const size_t poolSize = 2;

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

    MemoryAllocator<memorySize> allocator_;
    typedef MemoryPool<memorySize> TestingPool;
    TestingPool* pool_;
};


TEST_F(MemoryPoolTest, testInitialize)
{
    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(poolSize, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testAcquire)
{
    void* resource = pool_->acquire();
    EXPECT_TRUE(0 != resource);

    EXPECT_EQ(1, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(poolSize - 1, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testRelease)
{
    void* resource = pool_->acquire();

    pool_->release(resource);
    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(poolSize, pool_->getInactiveResourceCount());
}


TEST_F(MemoryPoolTest, testMassAcquireAndRelease)
{
    const int acquireCount = 10;
    void* resources[acquireCount];

    for (int i = 0; i < acquireCount; ++i) {
        resources[i] = pool_->acquire();
    }

    EXPECT_EQ(acquireCount, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(0, int(pool_->getInactiveResourceCount()));

    for (int i = acquireCount - 1; i >= 0; --i) {
        pool_->release(resources[i]);
    }

    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(10, int(pool_->getInactiveResourceCount()));
}


TEST_F(MemoryPoolTest, testDestroy)
{
    for (int i = 0; i < 10; ++i) {
        (void)pool_->acquire();
    }

    pool_->destroy();

    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(0, int(pool_->getInactiveResourceCount()));
}

