#ifndef SRPC_VECTORSTREAMBUFFER_H
#define SRPC_VECTORSTREAMBUFFER_H

#include "../StreamBuffer.h"
#include "../Exception.h"
#include "../ContainerTypes.h"
#include "../utility/StlUtils.h"
#include <cassert>

namespace srpc {

/** @addtogroup serialization
* @{
*/

/**
 * @class VectorStreamBuffer
 *
 * std::vector 기반의 StreamBuffer
 * - 효율을 위하여 read/write pointer를 사용함
 */
class VectorStreamBuffer : public StreamBuffer
{
    typedef Vector<Item> Buffer;
    typedef Buffer::size_type Index;

    enum { defaultStreamCapacity = 512 };
public:
    VectorStreamBuffer(
        size_t defaultCapacity = defaultStreamCapacity) :
        readIndex_(0),
        writeIndex_(0) {
        buffer_.reserve(defaultCapacity);
    }

    virtual void push(Item item) {
        reserve(sizeof(item));
        buffer_.insert(buffer_.end(), item);
        ++writeIndex_;
    }

    virtual void pop() {
        if (! empty()) {
            ++readIndex_;
            if (empty()) {
                // TODO: 매번 버퍼를 비우므로 비효율적이다
                reset();
            }
        }
    }

    virtual void copyFrom(const Item* buffer, size_t bufferSize) {
        reserve(bufferSize);
        buffer_.insert(buffer_.end(), buffer, buffer + bufferSize);
        writeIndex_ += bufferSize;
    }

    virtual void copyTo(Item* buffer, size_t bufferSize) {
        if (size() < bufferSize) {
            throw StreamingException(__FILE__, __LINE__,
                "buffer is corrupted");
        }

        copyVector(buffer_.begin() + readIndex_, buffer, bufferSize);
        readIndex_ += bufferSize;
        
        if (empty()) {
            // TODO: 매번 버퍼를 비우므로 비효율적이다
            reset();
        }
    }

    virtual void reset() {
        readIndex_ = writeIndex_ = 0;
        buffer_.resize(0);
        // CAUTION: buffer_.clear()를 호출하면 안됨. 내부적으로 메모리를
        // 해제하여 capacity()가 0으로 설정되어 버림.
    }

    virtual Item front() const {
        assert(! empty());
        return buffer_[readIndex_];
    }

    virtual Item& front() {
        assert(! empty());
        return buffer_[readIndex_];
    }

    virtual Item back() const {
        assert(! empty());
        return buffer_[writeIndex_ - 1];
    }

    virtual Item& back() {
        assert(! empty());
        return buffer_[writeIndex_ - 1];
    }

    virtual size_t size() const {
        return writeIndex_ - readIndex_;
    }

    virtual bool empty() const {
        return writeIndex_ == readIndex_;
    }

    virtual size_t space() const {
        return buffer_.capacity() - writeIndex_;
    }
public:
    bool isValid() const {
        return size() == buffer_.size();
    }
private:
    void reserve(size_t neededSize) {
        if (space() < neededSize) {
            buffer_.reserve(buffer_.capacity() * 2);
            if (space() < neededSize) {
                throw StreamingException(__FILE__, __LINE__, "out of memory");
            }
        }
    }
private:
    Index readIndex_;
    Index writeIndex_;
    Buffer buffer_;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_VECTORSTREAMBUFFER_H
