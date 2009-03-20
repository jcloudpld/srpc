#include "stdafx.h"
#include <nsrpc/utility/MessageBlockManager.h>

using namespace nsrpc;

/**
 * @class TestMessageBlockManager
 */
class TestMessageBlockManager : public NoSynchMessageBlockManager
{
public:
    TestMessageBlockManager(size_t poolSize, size_t blockSize) :
        NoSynchMessageBlockManager(poolSize, blockSize) {}

    const MessageBlockAllocator& getMessageBlockAllocator() const {
        return messageBlockAllocator_;
    }

    const DataBlockAllocator& getDataBlockAllocator() const {
        return dataBlockAllocator_;
    }

#ifdef USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER
    const BufferAllocator& getBufferAllocator() const {
        return bufferAllocator_;
    }
#endif
};


/**
* @class MessageBlockManagerTest
*
* MessageBlockManager Test
*/
class MessageBlockManagerTest : public testing::Test
{
private:
    virtual void SetUp() {
        manager_ = new TestMessageBlockManager(poolSize, blockSize);
    }

    virtual void TearDown() {
        delete manager_;
    }

protected:
    enum {
        poolSize = 2,
        blockSize = 100
    };

    TestMessageBlockManager* manager_;
};


TEST_F(MessageBlockManagerTest, testInitialize)
{
    EXPECT_EQ(poolSize,
        manager_->getMessageBlockAllocator().getCachedMemoryCount());
    EXPECT_EQ(poolSize,
        manager_->getDataBlockAllocator().getCachedMemoryCount());
#ifdef USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER
    EXPECT_EQ(poolSize,
        manager_->getBufferAllocator().getCachedMemoryCount());
#endif
}


TEST_F(MessageBlockManagerTest, testAcquire)
{
    ACE_Message_Block* mblock = manager_->create(blockSize);
    EXPECT_TRUE(0 != mblock);

    EXPECT_EQ(poolSize - 1,
        manager_->getMessageBlockAllocator().getCachedMemoryCount());
    EXPECT_EQ(poolSize - 1,
        manager_->getDataBlockAllocator().getCachedMemoryCount());
#ifdef USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER
    EXPECT_EQ(poolSize - 1,
        manager_->getBufferAllocator().getCachedMemoryCount());
#endif
}


TEST_F(MessageBlockManagerTest, testRelease)
{
    manager_->create(blockSize)->release();

    EXPECT_EQ(poolSize,
        manager_->getMessageBlockAllocator().getCachedMemoryCount());
    EXPECT_EQ(poolSize,
        manager_->getDataBlockAllocator().getCachedMemoryCount());
#ifdef USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER
    EXPECT_EQ(poolSize,
        manager_->getBufferAllocator().getCachedMemoryCount());
#endif
}


TEST_F(MessageBlockManagerTest, testDuplicate)
{
    ACE_Message_Block* mblock = manager_->create(blockSize);
    ACE_Message_Block* duplicated = mblock->duplicate();
    EXPECT_TRUE(0 != duplicated);

    mblock->release();
    duplicated->release();

    EXPECT_EQ(poolSize,
        manager_->getMessageBlockAllocator().getCachedMemoryCount());
    EXPECT_EQ(poolSize,
        manager_->getDataBlockAllocator().getCachedMemoryCount());
#ifdef USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER
    EXPECT_EQ(poolSize,
        manager_->getBufferAllocator().getCachedMemoryCount());
#endif
}


TEST_F(MessageBlockManagerTest, testClone)
{
    ACE_Message_Block* mblock = manager_->create(blockSize);
    ACE_Message_Block* cloned = mblock->clone();
    EXPECT_TRUE(0 != cloned);

    EXPECT_EQ(poolSize - 2,
        manager_->getMessageBlockAllocator().getCachedMemoryCount());
    EXPECT_EQ(poolSize - 2,
        manager_->getDataBlockAllocator().getCachedMemoryCount());
#ifdef USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER
    EXPECT_EQ(poolSize - 2,
        manager_->getBufferAllocator().getCachedMemoryCount());
#endif

    mblock->release();
    cloned->release();

    EXPECT_EQ(poolSize,
        manager_->getMessageBlockAllocator().getCachedMemoryCount());
    EXPECT_EQ(poolSize,
        manager_->getDataBlockAllocator().getCachedMemoryCount());
#ifdef USE_VARIOUS_MEMORY_ALLOCATOR_IN_MESSAGE_BLOCK_MANAGER
    EXPECT_EQ(poolSize,
        manager_->getBufferAllocator().getCachedMemoryCount());
#endif
}
