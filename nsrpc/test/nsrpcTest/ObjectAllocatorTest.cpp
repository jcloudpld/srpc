#include "stdafx.h"
#include <nsrpc/utility/ObjectAllocator.h>

using namespace nsrpc;

/**
* @class ObjectAllocatorTest
*
* ObjectAllocator Test
*/
class ObjectAllocatorTest : public testing::Test
{
private:
    virtual void SetUp() {
        allocator_ = new IntegerAllocator(poolSize);
    }

    virtual void TearDown() {
        delete allocator_;
    }

protected:
    enum {
        poolSize = 2,
        blockSize = sizeof(int)
    };

    typedef ObjectAllocator<int> IntegerAllocator;
    IntegerAllocator* allocator_;
};


TEST_F(ObjectAllocatorTest, testInitialize)
{
    EXPECT_EQ(poolSize, allocator_->getCachedMemoryCount());
}


TEST_F(ObjectAllocatorTest, testMalloc)
{
    void* memory = allocator_->malloc(blockSize);
    EXPECT_TRUE(0 != memory);

    EXPECT_EQ(poolSize - 1, allocator_->getCachedMemoryCount());
}


TEST_F(ObjectAllocatorTest, testCalloc)
{
    void* memory = allocator_->calloc(blockSize);
    EXPECT_TRUE(0 != memory);

    EXPECT_EQ(poolSize - 1, allocator_->getCachedMemoryCount());
}


TEST_F(ObjectAllocatorTest, testMallocBigger)
{
    void* memory = allocator_->malloc(blockSize * 2);
    EXPECT_TRUE(0 == memory);
}


TEST_F(ObjectAllocatorTest, testMallocSmaller)
{
    void* memory = allocator_->malloc(blockSize / 2);
    EXPECT_TRUE(0 != memory);

    EXPECT_EQ(poolSize - 1, allocator_->getCachedMemoryCount());
}


TEST_F(ObjectAllocatorTest, testFree)
{
    allocator_->free(allocator_->malloc(blockSize));

    EXPECT_EQ(poolSize, allocator_->getCachedMemoryCount());
}


TEST_F(ObjectAllocatorTest, testMultipleMallocFree)
{
    const int count = 1000;
    void* memories[count];
    for (int i = 0; i < count; ++i) {
        memories[i] = allocator_->malloc(blockSize);
    }
    EXPECT_EQ(0, allocator_->getCachedMemoryCount());

    for (int i = 0; i < count; ++i) {
        allocator_->free(memories[i]);
    }
    EXPECT_EQ(poolSize + 1000 - 2, allocator_->getCachedMemoryCount());
}
