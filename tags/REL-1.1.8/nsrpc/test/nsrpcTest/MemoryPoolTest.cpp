#include "stdafx.h"
#include <nsrpc/utility/MemoryPool.h>

using namespace nsrpc;

/**
* @class MemoryPoolTest
*
* MemoryPool Test
*/
class MemoryPoolTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MemoryPoolTest);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testAcquire);
    CPPUNIT_TEST(testRelease);
    CPPUNIT_TEST(testMassAcquireAndRelease);
    CPPUNIT_TEST(testDestroy);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testInitialize();
    void testAcquire();
    void testRelease();
    void testMassAcquireAndRelease();
    void testDestroy();
private:
    enum {
        memorySize = 128,
        poolSize = 2
    };

    MemoryAllocator<memorySize> allocator_;
    typedef MemoryPool<memorySize> TestingPool;
    TestingPool* pool_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MemoryPoolTest);

void MemoryPoolTest::setUp()
{
    pool_ = new TestingPool(poolSize, allocator_);
    pool_->initialize();
}


void MemoryPoolTest::tearDown()
{
    delete pool_;
}


void MemoryPoolTest::testInitialize()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        0,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        static_cast<int>(poolSize),
        static_cast<int>(pool_->getInactiveResourceCount()));
}


void MemoryPoolTest::testAcquire()
{
    void* resource = pool_->acquire();
    CPPUNIT_ASSERT(0 != resource);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        1,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        poolSize - 1,
        static_cast<int>(pool_->getInactiveResourceCount()));
}


void MemoryPoolTest::testRelease()
{
    void* resource = pool_->acquire();

    pool_->release(resource);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        0,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        static_cast<int>(poolSize),
        static_cast<int>(pool_->getInactiveResourceCount()));
}


void MemoryPoolTest::testMassAcquireAndRelease()
{
    const int acquireCount = 10;
    void* resources[acquireCount];

    for (int i = 0; i < acquireCount; ++i) {
        resources[i] = pool_->acquire();
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        acquireCount,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        0,
        static_cast<int>(pool_->getInactiveResourceCount()));

    for (int i = acquireCount - 1; i >= 0; --i) {
        pool_->release(resources[i]);
    }

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        0,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        10,
        static_cast<int>(pool_->getInactiveResourceCount()));
}


void MemoryPoolTest::testDestroy()
{
    for (int i = 0; i < 10; ++i) {
        (void)pool_->acquire();
    }

    pool_->destroy();

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        0,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        0,
        static_cast<int>(pool_->getInactiveResourceCount()));
}