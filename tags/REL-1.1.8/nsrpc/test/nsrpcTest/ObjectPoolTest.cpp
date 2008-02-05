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


/**
* @class ObjectPoolTest
*
* ObjectPool Test
*/
class ObjectPoolTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ObjectPoolTest);
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
        poolSize = 2
    };

    IntegerAllocator allocator_;
    typedef ObjectPool<int, IntegerAllocator> IntegerPool;
    IntegerPool* pool_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ObjectPoolTest);

void ObjectPoolTest::setUp()
{
    pool_ = new IntegerPool(poolSize, allocator_);
    pool_->initialize();
}


void ObjectPoolTest::tearDown()
{
    delete pool_;
}


void ObjectPoolTest::testInitialize()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        0,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        static_cast<int>(poolSize),
        static_cast<int>(pool_->getInactiveResourceCount()));
}


void ObjectPoolTest::testAcquire()
{
    int* resource = pool_->acquire();
    CPPUNIT_ASSERT(0 != resource);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        1,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        poolSize - 1,
        static_cast<int>(pool_->getInactiveResourceCount()));
}


void ObjectPoolTest::testRelease()
{
    int* resource = pool_->acquire();

    pool_->release(resource);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("active resource count",
        0,
        static_cast<int>(pool_->getActiveResourceCount()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("inactive resource count",
        static_cast<int>(poolSize),
        static_cast<int>(pool_->getInactiveResourceCount()));
}


void ObjectPoolTest::testMassAcquireAndRelease()
{
    const int acquireCount = 10;
    int* resources[acquireCount];

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


void ObjectPoolTest::testDestroy()
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
