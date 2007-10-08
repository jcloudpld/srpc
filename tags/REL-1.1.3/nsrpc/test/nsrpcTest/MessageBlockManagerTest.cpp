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

    const BufferAllocator& getBufferAllocator() const {
        return bufferAllocator_;
    }
};


/**
* @class MessageBlockManagerTest
*
* MessageBlockManager Test
*/
class MessageBlockManagerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MessageBlockManagerTest);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testAcquire);
    CPPUNIT_TEST(testRelease);
    CPPUNIT_TEST(testDuplicate);
    CPPUNIT_TEST(testClone);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testInitialize();
    void testAcquire();
    void testRelease();
    void testDuplicate();
    void testClone();
private:
    enum {
        poolSize = 2,
        blockSize = 100
    };

    TestMessageBlockManager* manager_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MessageBlockManagerTest);

void MessageBlockManagerTest::setUp()
{
    manager_ = new TestMessageBlockManager(poolSize, blockSize);
}


void MessageBlockManagerTest::tearDown()
{
    delete manager_;
}


void MessageBlockManagerTest::testInitialize()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached message block count",
        static_cast<int>(poolSize),
        static_cast<int>(
            manager_->getMessageBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached data block count",
        static_cast<int>(poolSize),
        static_cast<int>(
            manager_->getDataBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached buffer count",
        static_cast<int>(poolSize),
        static_cast<int>(
            manager_->getBufferAllocator().getCachedMemoryCount()));
}


void MessageBlockManagerTest::testAcquire()
{
    ACE_Message_Block* mblock = manager_->create(blockSize);
    CPPUNIT_ASSERT_MESSAGE("mblock is not null",
        0 != mblock);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached message block count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(
            manager_->getMessageBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached data block count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(
            manager_->getDataBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached buffer count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(
            manager_->getBufferAllocator().getCachedMemoryCount()));
}


void MessageBlockManagerTest::testRelease()
{
    manager_->create(blockSize)->release();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached message block count",
        static_cast<int>(poolSize),
        static_cast<int>(
            manager_->getMessageBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached data block count",
        static_cast<int>(poolSize),
        static_cast<int>(
            manager_->getDataBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached buffer count",
        static_cast<int>(poolSize),
        static_cast<int>(
            manager_->getBufferAllocator().getCachedMemoryCount()));
}


void MessageBlockManagerTest::testDuplicate()
{
    ACE_Message_Block* mblock = manager_->create(blockSize);
    ACE_Message_Block* duplicated = mblock->duplicate();
    CPPUNIT_ASSERT_MESSAGE("duplicated is not null",
        0 != duplicated);

    mblock->release();
    duplicated->release();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached message block count",
        static_cast<int>(poolSize),
        static_cast<int>(
        manager_->getMessageBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached data block count",
        static_cast<int>(poolSize),
        static_cast<int>(
        manager_->getDataBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached buffer count",
        static_cast<int>(poolSize),
        static_cast<int>(
        manager_->getBufferAllocator().getCachedMemoryCount()));
}


void MessageBlockManagerTest::testClone()
{
    ACE_Message_Block* mblock = manager_->create(blockSize);
    ACE_Message_Block* cloned = mblock->clone();
    CPPUNIT_ASSERT_MESSAGE("cloned is not null",
        0 != cloned);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached message block count",
        static_cast<int>(poolSize - 2),
        static_cast<int>(
        manager_->getMessageBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached data block count",
        static_cast<int>(poolSize - 2),
        static_cast<int>(
        manager_->getDataBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached buffer count",
        static_cast<int>(poolSize - 2),
        static_cast<int>(
        manager_->getBufferAllocator().getCachedMemoryCount()));

    mblock->release();
    cloned->release();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached message block count",
        static_cast<int>(poolSize),
        static_cast<int>(
        manager_->getMessageBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached data block count",
        static_cast<int>(poolSize),
        static_cast<int>(
        manager_->getDataBlockAllocator().getCachedMemoryCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached buffer count",
        static_cast<int>(poolSize),
        static_cast<int>(
        manager_->getBufferAllocator().getCachedMemoryCount()));
}
