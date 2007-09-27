#ifndef NSRPC_OBJECTALLOCATOR_H
#define NSRPC_OBJECTALLOCATOR_H

#include "detail/BaseAllocator.h"
#include <ace/Guard_T.h>
#include <ace/Null_Mutex.h>
#include <vector>

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * @class ObjectAllocator
 *
 * 고정 크기의 메모리 할당자
 * - Mutex: Mutex 타입(예, ACE_Null_Mutex, ACE_Thread_Mutex, ...)
 * - 풀이 가득 찰 경우 poolSize 만큼 여유를 더 만든다.
 * - 메모리 복사가 일어나지 않도록 vector의 끝에서 input/output 처리.
 */
template <typename Object, class Mutex = ACE_Null_Mutex>
class ObjectAllocator : public BaseAllocator
{
    typedef std::vector<Object*> Pool;
public:
    /**
     * ctor
     * @param poolSize Pool 크기.
     */
    ObjectAllocator(size_t poolSize) :
        poolSize_(poolSize) {
        preallocate();
    }

    virtual ~ObjectAllocator() {
        destroy();
    }
public:
    // = ACE_Allocator overriding
    virtual void* malloc(size_t nbytes) {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        if (nbytes > sizeof(Object)) {
            return 0;
        }

        if (pool_.empty()) {
            preallocate();
        }

        void* memory = pool_.back();
        pool_.pop_back();

        assert(memory != 0);
        return memory;
    }

    virtual void* calloc(size_t nbytes, char initial_value = '\0') {
        void* memory = malloc(nbytes);
        if (memory != 0) {
            ACE_OS::memset(memory, initial_value, nbytes);
        }
        return memory;
    }

    virtual void* calloc(size_t n_elem, size_t elem_size,
        char initial_value = '\0') {
        return ObjectAllocator::calloc(n_elem * elem_size,
            initial_value);
    }

    virtual void free(void* ptr) {
        if (ptr != 0) {
            ACE_GUARD(Mutex, monitor, lock_);

            pool_.push_back(static_cast<Object*>(ptr));
        }
    }
public:
    size_t getCachedMemoryCount() const {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        return pool_.size();
    }
private:
    void preallocate() {
        if ((pool_.capacity() - pool_.size()) < poolSize_) {
            pool_.reserve(pool_.size() + poolSize_);
        }
        for (size_t i = 0; i < poolSize_; ++i) {
            pool_.push_back(static_cast<Object*>(std::malloc(sizeof(Object))));
        }
    }

    void destroy() {
        ACE_GUARD(Mutex, monitor, lock_);

        Pool::iterator pos = pool_.begin();
        const Pool::iterator end = pool_.end();
        for (; pos != end; ++pos) {
            std::free(*pos);
        }
        pool_.clear();
    }
private:
    size_t poolSize_;
    Pool pool_;
    mutable Mutex lock_;
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_OBJECTALLOCATOR_H
