#include "stdafx.h"
#include <nsrpc/utility/ObjectAllocator.h>

using namespace nsrpc;

/**
* @class ObjectAllocatorTest
*
* ObjectAllocator Test
*/
class ObjectAllocatorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ObjectAllocatorTest);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testMalloc);
    CPPUNIT_TEST(testCalloc);
    CPPUNIT_TEST(testMallocBigger);
    CPPUNIT_TEST(testMallocSmaller);
    CPPUNIT_TEST(testFree);
    CPPUNIT_TEST(testMultipleMallocFree);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testInitialize();
    void testMalloc();
    void testCalloc();
    void testMallocBigger();
    void testMallocSmaller();
    void testFree();
    void testMultipleMallocFree();
private:
    enum {
        poolSize = 2,
        blockSize = sizeof(int)
    };

    typedef ObjectAllocator<int> IntegerAllocator;
    IntegerAllocator* allocator_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ObjectAllocatorTest);

void ObjectAllocatorTest::setUp()
{
    allocator_ = new IntegerAllocator(poolSize);
}


void ObjectAllocatorTest::tearDown()
{
    delete allocator_;
}


void ObjectAllocatorTest::testInitialize()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void ObjectAllocatorTest::testMalloc()
{
    void* memory = allocator_->malloc(blockSize);
    CPPUNIT_ASSERT_MESSAGE("memory is not null",
        0 != memory);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void ObjectAllocatorTest::testCalloc()
{
    void* memory = allocator_->calloc(blockSize);
    CPPUNIT_ASSERT_MESSAGE("memory is not null",
        0 != memory);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void ObjectAllocatorTest::testMallocBigger()
{
    void* memory = allocator_->malloc(blockSize * 2);
    CPPUNIT_ASSERT_MESSAGE("memory is null",
        0 == memory);
}


void ObjectAllocatorTest::testMallocSmaller()
{
    void* memory = allocator_->malloc(blockSize / 2);
    CPPUNIT_ASSERT_MESSAGE("memory is not null",
        0 != memory);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void ObjectAllocatorTest::testFree()
{
    allocator_->free(allocator_->malloc(blockSize));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void ObjectAllocatorTest::testMultipleMallocFree()
{
    const int count = 1000;
    void* memories[count];
    for (int i = 0; i < count; ++i) {
        memories[i] = allocator_->malloc(blockSize);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(0),
        static_cast<int>(allocator_->getCachedMemoryCount()));

    for (int i = 0; i < count; ++i) {
        allocator_->free(memories[i]);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize + 1000 - 2),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}
