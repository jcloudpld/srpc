#include "stdafx.h"
#include <nsrpc/utility/VariousMemoryAllocator.h>

using namespace nsrpc;

/**
* @class VariousMemoryAllocatorTest
*
* VariousMemoryAllocator Test
*/
class VariousMemoryAllocatorTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(VariousMemoryAllocatorTest);
    CPPUNIT_TEST(testInitialize);
    CPPUNIT_TEST(testMalloc);
    CPPUNIT_TEST(testCalloc);
    CPPUNIT_TEST(testMallocBigger);
    CPPUNIT_TEST(testMallocSmaller);
    CPPUNIT_TEST(testFree);
    CPPUNIT_TEST(testMultipleVariousMemoryMallocFree);
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
    void testMultipleVariousMemoryMallocFree();
private:
    enum {
        poolSize = 2,
        blockSize = sizeof(int)
    };

    typedef VariousMemoryAllocator<ACE_Null_Mutex> IntegerAllocator;
    IntegerAllocator* allocator_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(VariousMemoryAllocatorTest);

void VariousMemoryAllocatorTest::setUp()
{
    allocator_ = new IntegerAllocator(poolSize, blockSize);
}


void VariousMemoryAllocatorTest::tearDown()
{
    delete allocator_;
}


void VariousMemoryAllocatorTest::testInitialize()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void VariousMemoryAllocatorTest::testMalloc()
{
    void* memory = allocator_->malloc(blockSize);
    CPPUNIT_ASSERT_MESSAGE("memory is not null",
        0 != memory);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void VariousMemoryAllocatorTest::testCalloc()
{
    void* memory = allocator_->calloc(blockSize);
    CPPUNIT_ASSERT_MESSAGE("memory is not null",
        0 != memory);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void VariousMemoryAllocatorTest::testMallocBigger()
{
    void* memory = allocator_->malloc(blockSize * 2);
    CPPUNIT_ASSERT_MESSAGE("memory is not null",
        0 != memory);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void VariousMemoryAllocatorTest::testMallocSmaller()
{
    void* memory = allocator_->malloc(blockSize / 2);
    CPPUNIT_ASSERT_MESSAGE("memory is not null",
        0 != memory);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize - 1),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void VariousMemoryAllocatorTest::testFree()
{
    allocator_->free(allocator_->malloc(blockSize));

    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}


void VariousMemoryAllocatorTest::testMultipleVariousMemoryMallocFree()
{
    const int count = 1000;
    void* memories[count];
    for (int i = 0; i < count; ++i) {
        memories[i] = allocator_->malloc((count % blockSize) + 2);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(0),
        static_cast<int>(allocator_->getCachedMemoryCount()));

    for (int i = 0; i < count; ++i) {
        allocator_->free(memories[i]);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("cached memory count",
        static_cast<int>(poolSize + count - 2),
        static_cast<int>(allocator_->getCachedMemoryCount()));
}
