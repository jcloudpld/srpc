#ifndef SRPC_DUMMYSTREAMBUFFER_H
#define SRPC_DUMMYSTREAMBUFFER_H

#include <srpc/detail/VectorStreamBuffer.h>
#include <srpc/Exception.h>

/**
 * @class DummyStreamBuffer
 */
class DummyStreamBuffer : public srpc::VectorStreamBuffer
{
public:
    DummyStreamBuffer() :
        isPushError_(false) {}

    virtual void push(Item item) {
        if (isPushError_) {
            throw srpc::StreamingException(__FILE__, __LINE__, "push error");
        }
        else {
            srpc::VectorStreamBuffer::push(item);
        }
    }

    virtual void copyFrom(const Item* buffer, size_t bufferSize) {
        if (isPushError_) {
            throw srpc::StreamingException(
                __FILE__, __LINE__, "copyFrom error");
        }
        else {
            srpc::VectorStreamBuffer::copyFrom(buffer, bufferSize);
        }
    }

    void setPushError() {
        isPushError_ = true;
    }
private:
    bool isPushError_;
};

#endif // SRPC_DUMMYSTREAMBUFFER_H
