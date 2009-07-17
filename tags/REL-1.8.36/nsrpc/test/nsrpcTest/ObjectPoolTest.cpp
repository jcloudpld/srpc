#include "stdafx.h"
#include <nsrpc/utility/ObjectPool.h>

using namespace nsrpc;

/**
* @class IntegerAllocator
*/
class IntegerAllocator
{
public:
    int* malloc() {
        return new int(1);
    }

    void free(int* resource) {
        delete resource;
    }
};


static const size_t poolSize = 2;

/**
* @class ObjectPoolTest
*
* ObjectPool Test
*/
class ObjectPoolTest : public testing::Test
{
private:
    virtual void SetUp() {
        pool_ = new IntegerPool(poolSize, allocator_);
        pool_->initialize();
    }

    virtual void TearDown() {
        delete pool_;
    }

protected:
    IntegerAllocator allocator_;
    typedef ObjectPool<int, IntegerAllocator> IntegerPool;
    IntegerPool* pool_;
};


TEST_F(ObjectPoolTest, testInitialize)
{
    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(poolSize, pool_->getInactiveResourceCount());
}


TEST_F(ObjectPoolTest, testAcquire)
{
    int* resource = pool_->acquire();
    EXPECT_TRUE(0 != resource);

    EXPECT_EQ(1, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(poolSize - 1, pool_->getInactiveResourceCount());
}


TEST_F(ObjectPoolTest, testRelease)
{
    int* resource = pool_->acquire();

    pool_->release(resource);
    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(poolSize, pool_->getInactiveResourceCount());
}


TEST_F(ObjectPoolTest, testMassAcquireAndRelease)
{
    const size_t acquireCount = 10;
    int* resources[acquireCount];

    for (size_t i = 0; i < acquireCount; ++i) {
        resources[i] = pool_->acquire();
    }

    EXPECT_EQ(acquireCount, pool_->getActiveResourceCount());
    EXPECT_EQ(0, int(pool_->getInactiveResourceCount()));

    for (int i = acquireCount - 1; i >= 0; --i) {
        pool_->release(resources[i]);
    }

    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(10, int(pool_->getInactiveResourceCount()));
}


TEST_F(ObjectPoolTest, testDestroy)
{
    for (int i = 0; i < 10; ++i) {
        (void)pool_->acquire();
    }

    pool_->destroy();

    EXPECT_EQ(0, int(pool_->getActiveResourceCount()));
    EXPECT_EQ(0, int(pool_->getInactiveResourceCount()));
}

