#ifndef NSRPC_MESSAGEBLOCKSTREAMBUFFER_H
#define NSRPC_MESSAGEBLOCKSTREAMBUFFER_H

#include <srpc/StreamBuffer.h>
#include <srpc/Exception.h>
#include <srpc/utility/StlUtils.h>
#include <ace/Message_Block.h>
#include <cassert>

namespace nsrpc
{

/** @addtogroup serialization
* @{
*/

/**
 * @class MessageBlockStreamBuffer
 *
 * ACE_Message_Block 기반의 StreamBuffer
 * - ACE_Message_Block을 소유하지 않는다(메모리를 해제하지 않는다)
 */
class MessageBlockStreamBuffer : public srpc::StreamBuffer
{
public:
    MessageBlockStreamBuffer(ACE_Message_Block* block) :
        block_(block) {
        assert(block != 0);
    }

    /// 내부 버퍼를 newBlock으로 대체한다.
    void reset(ACE_Message_Block* newBlock) {
        assert(newBlock != 0);
        block_ = newBlock;
    }

    ACE_Message_Block* getBlock() {
        return block_;
    }
public:
    virtual void push(Item item) {
        reserve(sizeof(item));
        void* wrPtr = block_->wr_ptr();
        *static_cast<Item*>(wrPtr) = item;
        block_->wr_ptr(sizeof(item));
    }

    virtual void pop() {
        if (block_->length() != 0) {
            block_->rd_ptr(sizeof(Item));
            if (block_->length() == 0) {
                reset();
            }
        }
    }

    virtual void copyFrom(const Item* block, size_t blockSize) {
        reserve(blockSize);
        const void* voidBlock = block;
        block_->copy(static_cast<const char*>(voidBlock),
            sizeof(Item) * blockSize);
    }

    virtual void copyTo(Item* block, size_t blockSize) {
        if (block_->length() < blockSize) {
            throw srpc::StreamingException(__FILE__, __LINE__,
                "block is corrupted");
        }

        srpc::copyVector(block_->rd_ptr(), block, blockSize);
        block_->rd_ptr(blockSize);

        if (block_->length() == 0) {
            reset();
        }
    }

    virtual void reset() {
        block_->reset();
    }

    virtual Item front() const {
        return const_cast<MessageBlockStreamBuffer*>(this)->front();
    }

    virtual Item& front() {
        assert(! empty());
        void* rdPtr = block_->rd_ptr();
        return *static_cast<Item*>(rdPtr);
    }

    virtual Item back() const {
        return const_cast<MessageBlockStreamBuffer*>(this)->back();
    }

    virtual Item& back() {
        assert(! empty());
        void* wrPtr = (block_->wr_ptr() - sizeof(Item));
        return *static_cast<Item*>(wrPtr);
    }

    virtual size_t size() const {
        return block_->length();
    }

    virtual bool empty() const {
        return block_->length() == 0;
    }

    virtual size_t space() const {
        return block_->space();
    }
public:
    bool isValid() const {
        return size() == block_->total_length();
    }
private:
    void reserve(size_t neededSize) {
        if (block_->space() < neededSize) {
            block_->size(block_->capacity() + (neededSize * 2));
            if (block_->space() < neededSize) {
                throw srpc::StreamingException(
                    __FILE__, __LINE__, "out of memory");
            }
        }
    }
private:
    ACE_Message_Block* block_;
};

/** @} */ // addtogroup serialization

} // namespace nsrpc

#endif // NSRPC_MESSAGEBLOCKSTREAMBUFFER_H
