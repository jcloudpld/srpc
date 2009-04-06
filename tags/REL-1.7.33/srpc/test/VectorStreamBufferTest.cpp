#include "stdafx.h"
#include <srpc/detail/VectorStreamBuffer.h>

using namespace srpc;

const size_t space_ = 10;

/**
* @class VectorStreamBufferTest
*
* std::vector based stream buffer Test
*/
class VectorStreamBufferTest : public testing::Test
{
private:
    virtual void SetUp() {
        buffer_ = new VectorStreamBuffer(space_);
    }

    virtual void TearDown() {
        delete buffer_;
    }

protected:
    StreamBuffer* buffer_;
};


TEST_F(VectorStreamBufferTest, testEmpty)
{
    EXPECT_TRUE(buffer_->empty()) << "empty";
    EXPECT_EQ(0, static_cast<int>(buffer_->size())) << "0 size";
}


TEST_F(VectorStreamBufferTest, testPush)
{
    buffer_->push(1);
    EXPECT_EQ(1, static_cast<int>(buffer_->size())) << "1 size";
    buffer_->push(2);
    EXPECT_EQ(2, static_cast<int>(buffer_->size())) << "2 size";
}


TEST_F(VectorStreamBufferTest, testRead)
{
    buffer_->push(1);
    EXPECT_EQ(1, buffer_->back()) << "back(size 1)";
    buffer_->push(2);
    EXPECT_EQ(2, buffer_->back()) << "back(size 2)";
    EXPECT_EQ(1, buffer_->front()) << "front";
}


TEST_F(VectorStreamBufferTest, testPop)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->pop();
    EXPECT_EQ(2, buffer_->front()) << "front";
    buffer_->pop();
    EXPECT_EQ(true, buffer_->empty()) << "empty";

    buffer_->push(3);
    EXPECT_EQ(1, static_cast<int>(buffer_->size())) << "1 size";
}


TEST_F(VectorStreamBufferTest, testReset)
{
    buffer_->push(1);
    buffer_->push(2);
    buffer_->reset();
    EXPECT_TRUE(buffer_->empty());
    EXPECT_EQ(0, static_cast<int>(buffer_->size()));

    EXPECT_TRUE(static_cast<VectorStreamBuffer*>(buffer_)->isValid()) <<
        "must be valid";
}


TEST_F(VectorStreamBufferTest, testSpace)
{
    EXPECT_EQ(space_, buffer_->space());
    buffer_->push(1);
    buffer_->push(2);
    EXPECT_EQ(space_ - 2, buffer_->space());

    buffer_->pop();
    buffer_->pop();
    EXPECT_EQ(space_, buffer_->space());
}


TEST_F(VectorStreamBufferTest, testExtendedSpace)
{
    EXPECT_EQ(space_, buffer_->space());

    for (size_t i = 0; i < (space_ * 2); ++i) {
        buffer_->push(1);
    }
    EXPECT_EQ(0, static_cast<int>(buffer_->space()));

    buffer_->reset();
    EXPECT_EQ(space_ * 2, buffer_->space());
}


TEST_F(VectorStreamBufferTest, testCopyFrom)
{
    const Int32 value = -12345;
    const void* valuePtr = &value;
    buffer_->copyFrom(static_cast<const StreamBuffer::Item*>(valuePtr),
        sizeof(value));
    EXPECT_EQ(sizeof(value), buffer_->size());
}


TEST_F(VectorStreamBufferTest, testCopyTo)
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
    EXPECT_EQ(0, static_cast<int>(buffer_->size()));
    EXPECT_EQ(value, to);
}
