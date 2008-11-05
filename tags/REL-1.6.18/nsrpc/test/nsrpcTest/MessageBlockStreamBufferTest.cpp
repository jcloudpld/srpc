#include "stdafx.h"
#include <nsrpc/detail/MessageBlockStreamBuffer.h>
#include <nsrpc/utility/AceUtil.h>

using namespace srpc;
using namespace nsrpc;

/**
* @class MessageBlockStreamBufferTest
*
* ACE_Message_Block based stream buffer Test
*/
class MessageBlockStreamBufferTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(MessageBlockStreamBufferTest);
    CPPUNIT_TEST(testEmpty);
    CPPUNIT_TEST(testPush);
    CPPUNIT_TEST(testRead);
    CPPUNIT_TEST(testPop);
    CPPUNIT_TEST(testReset);
    CPPUNIT_TEST(testSpace);
    CPPUNIT_TEST(testExtendedSpace);
    CPPUNIT_TEST(testCopyFrom);
    CPPUNIT_TEST(testCopyTo);
    CPPUNIT_TEST(testResetWithNewBlock);
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
    void testResetWithNewBlock();
private:
    ACE_Message_Block* mblock_;
    MessageBlockStreamBuffer* buffer_;
    static const size_t space_ = 10;
};

CPPUNIT_TEST_SUITE_REGISTRATION(MessageBlockStreamBufferTest);

void MessageBlockStreamBufferTest::setUp()
{
    mblock_ = new ACE_Message_Block(space_);
    buffer_ = new MessageBlockStreamBuffer(mblock_);
}


void MessageBlockStreamBufferTest::tearDown()
{
    delete buffer_;
    delete mblock_;
}


void MessageBlockStreamBufferTest::testEmpty()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        true, buffer_->empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 size",
        0, static_cast<int>(buffer_->size()));
}


void MessageBlockStreamBufferTest::testPush()
{
    buffer_->push(1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 size",
        1, static_cast<int>(buffer_->size()));
    buffer_->push(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("2 size",
        2, static_cast<int>(buffer_->size()));

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testRead()
{
    buffer_->push(1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("back",
        1, static_cast<int>(buffer_->back()));
    buffer_->push(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("back",
        2, static_cast<int>(buffer_->back()));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("front",
        1, static_cast<int>(buffer_->front()));

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testPop()
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

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testReset()
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        true, buffer_->empty());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("0 size",
        0, static_cast<int>(buffer_->size()));

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testSpace()
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

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testExtendedSpace()
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        static_cast<int>(space_), static_cast<int>(buffer_->space()));

    for (size_t i = 0; i < (space_ * 2); ++i) {
        buffer_->push(1);
    }
    CPPUNIT_ASSERT_EQUAL_MESSAGE("full",
        0, static_cast<int>(buffer_->space()));

    buffer_->reset();
    CPPUNIT_ASSERT_EQUAL_MESSAGE("fully empty",
        static_cast<int>(space_) * 2, static_cast<int>(buffer_->space()));

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testCopyFrom()
{
    const Int32 value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));
    CPPUNIT_ASSERT_EQUAL_MESSAGE("size",
        static_cast<int>(sizeof(value)), static_cast<int>(buffer_->size()));

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testCopyTo()
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

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


void MessageBlockStreamBufferTest::testResetWithNewBlock()
{
    buffer_->push(1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 size",
        1, static_cast<int>(buffer_->size()));

    AceMessageBlockGuard second(new ACE_Message_Block(space_));
    buffer_->reset(second.get());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("empty",
        0, static_cast<int>(buffer_->size()));

    buffer_->push(2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("1 size",
        1, static_cast<int>(buffer_->size()));

    CPPUNIT_ASSERT_MESSAGE("isValid",
        static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}
