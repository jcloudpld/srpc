#ifndef NSRPC_VARIOUSMEMORYALLOCATOR_H
#define NSRPC_VARIOUSMEMORYALLOCATOR_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "detail/BaseAllocator.h"
#include <srpc/ContainerTypes.h>
#include <ace/Guard_T.h>
#include <algorithm>
#include <functional>

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * @class VariousMemoryAllocator
 *
 * �پ��� ũ���� �޸� �Ҵ���
 * - Mutex: Mutex Ÿ��(��, ACE_Null_Mutex, ACE_Thread_Mutex, ...)
 * - ���ϴ� ũ���� �޸𸮰� Ǯ�� ���� ��� ���� �Ҵ��Ѵ�.
 * - vector�� ����ؼ� �޸� ���� ����� ����������, multiset�̳� multimap��
 *   ����ϴ� �ͺ��� �ξ� ȿ�����̴�.
 * - Cache Hit Ratio�� ���� ������ �޶�����.
 * - Effective STL Item 23 ����.
 */
template <class Mutex>
class VariousMemoryAllocator : public BaseAllocator
{
private:
    /// memory block
    struct Block
    {
        enum { headerSize = sizeof(size_t) };

        size_t chunkSize_;
        void* chunk_;

        Block() :
            chunkSize_(0),
            chunk_(0) {}

        Block(size_t chunkSize) :
            chunkSize_(chunkSize) {
            chunk_ = std::malloc(chunkSize + headerSize);
            *static_cast<size_t*>(chunk_) = chunkSize;
        }

        Block(size_t chunkSize, void* block) :
            chunkSize_(chunkSize),
            chunk_(block) {}

        bool operator < (const Block& rhs) const {
            return chunkSize_ < rhs.chunkSize_;
        }

        void free() {
            std::free(chunk_);
            chunk_ = 0;
        }
    };
    typedef srpc::Vector<Block> Blocks;
public:
    /**
     * ctor
     * @param poolSize Pool ũ��.
     * @param chunkSize �Ҵ��� �޸� ũ��
     */
    VariousMemoryAllocator(size_t poolSize, size_t chunkSize) {
        preallocate(poolSize, chunkSize);
    }

    virtual ~VariousMemoryAllocator() {
        destroy();
    }
public:
    // = ACE_Allocator overriding
    virtual void* malloc(size_t nbytes) {
        void* chunk = 0;

        {
            ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

            const typename Blocks::iterator pos = find(nbytes);
            if (pos != cachedBlocks_.end()) {
                Block& block = *pos;
                assert(block.chunkSize_ >= nbytes);
                chunk = block.chunk_;
                cachedBlocks_.erase(pos); // ����� ���� ����!!!
            }
            else {
                chunk = Block(nbytes).chunk_;
            }
        }

        if (! chunk) {
            assert(false);
            return 0;
        }
        return static_cast<char*>(chunk) + Block::headerSize;
    }

    virtual void* calloc(size_t nbytes, char initial_value = '\0') {
        void* chunk = malloc(nbytes);
        if (chunk != 0) {
            ACE_OS::memset(chunk, initial_value, nbytes);
        }
        return chunk;
    }

    virtual void* calloc(size_t n_elem, size_t elem_size,
        char initial_value = '\0') {
        return VariousMemoryAllocator::calloc(n_elem * elem_size,
            initial_value);
    }

    virtual void free(void* ptr) {
        if (ptr != 0) {
            void* chunk = static_cast<char*>(ptr) - Block::headerSize;

            ACE_GUARD(Mutex, monitor, lock_);

            insert(Block(*static_cast<size_t*>(chunk), chunk));
        }
    }
public:
    size_t getCachedMemoryCount() const {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        return cachedBlocks_.size();
    }
private:
    void preallocate(size_t poolSize, size_t chunkSize) {
        assert(cachedBlocks_.empty());
        cachedBlocks_.reserve(cachedBlocks_.size() + poolSize);
        for (size_t i = 0; i < poolSize; ++i) {
            cachedBlocks_.push_back(Block(chunkSize));
        }
    }

    void insert(const Block& block) {
        const typename Blocks::iterator pos = find(block.chunkSize_);
        if (pos != cachedBlocks_.end()) {
            assert(! (*pos < block));
            cachedBlocks_.insert(pos, block);
        }
        else {
            cachedBlocks_.push_back(block);
        }
    }

    typename Blocks::iterator find(size_t chunkSize) {
        return std::lower_bound(cachedBlocks_.begin(), cachedBlocks_.end(),
            Block(chunkSize, 0));
    }

    void destroy() {
        std::for_each(cachedBlocks_.begin(), cachedBlocks_.end(),
            std::mem_fun_ref(&Block::free));
        cachedBlocks_.clear();
    }
private:
    Blocks cachedBlocks_;

    mutable Mutex lock_;
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_VARIOUSMEMORYALLOCATOR_H
