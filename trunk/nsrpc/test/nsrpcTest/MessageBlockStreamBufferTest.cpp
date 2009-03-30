#include "stdafx.h"
#include <nsrpc/detail/MessageBlockStreamBuffer.h>
#include <nsrpc/utility/AceUtil.h>

using namespace srpc;
using namespace nsrpc;

static const size_t initialSpace = 10;

/**
* @class MessageBlockStreamBufferTest
*
* ACE_Message_Block based stream buffer Test
*/
class MessageBlockStreamBufferTest : public testing::Test
{
private:
    virtual void SetUp() {
        mblock_ = new ACE_Message_Block(initialSpace);
        buffer_ = new MessageBlockStreamBuffer(mblock_);
    }

    virtual void TearDown() {
        delete buffer_;
        delete mblock_;
    }

protected:
    ACE_Message_Block* mblock_;
    MessageBlockStreamBuffer* buffer_;
};


TEST_F(MessageBlockStreamBufferTest, testEmpty)
{
    EXPECT_TRUE(buffer_->empty());
    EXPECT_EQ(0, buffer_->size());
}


TEST_F(MessageBlockStreamBufferTest, testPush)
{
    buffer_->push(1);
    EXPECT_EQ(1, buffer_->size());
    buffer_->push(2);
    EXPECT_EQ(2, buffer_->size());

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testRead)
{
    buffer_->push(1);
    EXPECT_EQ(1, buffer_->back());
    buffer_->push(2);
    EXPECT_EQ(2, buffer_->back());
    EXPECT_EQ(1, buffer_->front());

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testPop)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->pop();
    EXPECT_EQ(2, buffer_->front());
    buffer_->pop();
    EXPECT_EQ(true, buffer_->empty());

    buffer_->push(3);
    EXPECT_EQ(1, buffer_->size());

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testReset)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->reset();
    EXPECT_TRUE(buffer_->empty());
    EXPECT_EQ(0, buffer_->size());

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testSpace)
{
    EXPECT_EQ(initialSpace, buffer_->space());
    buffer_->push(1);
    buffer_->push(2);
    EXPECT_EQ(initialSpace - 2, buffer_->space());

    buffer_->pop();
    buffer_->pop();
    EXPECT_EQ(initialSpace, buffer_->space());

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testExtendedSpace)
{
    EXPECT_EQ(initialSpace, buffer_->space()) << "empty";

    for (size_t i = 0; i < (initialSpace * 2); ++i) {
        buffer_->push(1);
    }
    EXPECT_EQ(0, buffer_->space());

    buffer_->reset();
    EXPECT_EQ(initialSpace * 2, buffer_->space()) << "fully empty";

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testCopyFrom)
{
    const Int32 value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));
    EXPECT_EQ(sizeof(value), buffer_->size());

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testCopyTo)
{
    const Int32 value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));

    Int32 to = 0;
    void* toPtr = &to;
    buffer_->copyTo(static_cast<StreamBuffer::Item*>(toPtr),
        sizeof(to));
    EXPECT_TRUE(buffer_->empty());
    EXPECT_EQ(0, buffer_->size());
    EXPECT_EQ(value, to);

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}


TEST_F(MessageBlockStreamBufferTest, testResetWithNewBlock)
{
    buffer_->push(1);
    EXPECT_EQ(1, buffer_->size());

    AceMessageBlockGuard second(new ACE_Message_Block(initialSpace));
    buffer_->reset(second.get());
    EXPECT_EQ(0, buffer_->size());

    buffer_->push(2);
    EXPECT_EQ(1, buffer_->size());

    EXPECT_TRUE(static_cast<MessageBlockStreamBuffer*>(buffer_)->isValid());
}
