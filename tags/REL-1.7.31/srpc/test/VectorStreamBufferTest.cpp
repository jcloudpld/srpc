#include "stdafx.h"
#include <srpc/detail/VectorStreamBuffer.h>

using namespace srpc;

/**
* @class VectorStreamBufferTest
*
* std::vector based stream buffer Test
*/
class VectorStreamBufferTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(VectorStreamBufferTest);
    CPPUNIT_TEST(testEmpty);
    CPPUNIT_TEST(testPush);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST(testPop);
    CPPUNIT_TEST(testReset);
    CPPUNIT_TEST(testSpace);
    CPPUNIT_TEST(testExtendedSpace);
    CPPUNIT_TEST(testCopyFrom);
    CPPUNIT_TEST(testCopyTo);
    CPPUNIT_TEST_SUITE_END();
public:
    void setUp();
    void tearDown();
private:
    void testEmpty();
    void testPush();
    void testRead();
    void testPop();
    void testReset();
    void testSpace();
    void testExtendedSpace();
    void testCopyFrom();
    void testCopyTo();
private:
    StreamBuffer* buffer_;
    static const size_t space_ = 10;
};

CPPUNIT_TEST_SUITE_REGISTRATION(VectorStreamBufferTest);

void VectorStreamBufferTest::setUp()
{
    buffer_ = new VectorStreamBuffer(space_);
}


void VectorStreamBufferTest::tearDown()
{
    delete buffer_;
}


void VectorStreamBufferTest::testEmpty()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        true, buffer_->empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 size",
        0, static_cast<int>(buffer_->size()));
}


void VectorStreamBufferTest::testPush()
{
    buffer_->push(1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 size",
        1, static_cast<int>(buffer_->size()));
    buffer_->push(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 size",
        2, static_cast<int>(buffer_->size()));
}


void VectorStreamBufferTest::testRead()
{
    buffer_->push(1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("back",
        1, static_cast<int>(buffer_->back()));
    buffer_->push(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("back",
        2, static_cast<int>(buffer_->back()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("front",
        1, static_cast<int>(buffer_->front()));
}


void VectorStreamBufferTest::testPop()
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->pop();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("front",
        2, static_cast<int>(buffer_->front()));
    buffer_->pop();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        true, buffer_->empty());

    buffer_->push(3);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 size",
        1, static_cast<int>(buffer_->size()));
}


void VectorStreamBufferTest::testReset()
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        true, buffer_->empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 size",
        0, static_cast<int>(buffer_->size()));

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<VectorStreamBuffer*>(buffer_)->isValid());
}


void VectorStreamBufferTest::testSpace()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("space",
        static_cast<int>(space_), static_cast<int>(buffer_->space()));
    buffer_->push(1);
    buffer_->push(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("space",
        static_cast<int>(space_) - 2, static_cast<int>(buffer_->space()));

    buffer_->pop();
    buffer_->pop();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("space",
        static_cast<int>(space_), static_cast<int>(buffer_->space()));
}


void VectorStreamBufferTest::testExtendedSpace()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("space",
        static_cast<int>(space_), static_cast<int>(buffer_->space()));

    for (size_t i = 0; i < (space_ * 2); ++i) {
        buffer_->push(1);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("space",
        0, static_cast<int>(buffer_->space()));

    buffer_->reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("space",
        static_cast<int>(space_) * 2, static_cast<int>(buffer_->space()));
}


void VectorStreamBufferTest::testCopyFrom()
{
    const Int32 value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(sizeof(value)), static_cast<int>(buffer_->size()));
}


void VectorStreamBufferTest::testCopyTo()
{
    const Int32 value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));

    Int32 to = 0;
    void* toPtr = &to;
    buffer_->copyTo(static_cast<StreamBuffer::Item*>(toPtr),
        sizeof(to));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        true, buffer_->empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 size",
        0, static_cast<int>(buffer_->size()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("value",
        value, to);
}
