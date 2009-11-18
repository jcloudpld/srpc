#ifndef NSRPC_OBJECTALLOCATOR_H
#define NSRPC_OBJECTALLOCATOR_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "detail/BaseAllocator.h"
#include <srpc/ContainerTypes.h>
#include <srpc/Types.h>
#include <ace/Guard_T.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4267 4312 4996)
#endif
#include <ace/Null_Mutex.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * @class ObjectAllocator
 *
 * ���� ũ���� �޸� �Ҵ���
 * - Mutex: Mutex Ÿ��(��, ACE_Null_Mutex, ACE_Thread_Mutex, ...)
 * - Ǯ�� ���� �� ��� poolSize ��ŭ ������ �� �����.
 * - �޸� ���簡 �Ͼ�� �ʵ��� vector�� ������ input/output ó��.
 */
template <typename Object, class Mutex = ACE_Null_Mutex>
class ObjectAllocator : public BaseAllocator
{
    typedef srpc::Vector<Object*> Pool;
public:
    /**
     * ctor
     * @param poolSize Pool ũ��.
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
            pool_.push_back(static_cast<Object*>(
                static_cast<void*>(new srpc::UInt8[sizeof(Object)])));
        }
    }

    void destroy() {
        ACE_GUARD(Mutex, monitor, lock_);

        typename Pool::iterator pos = pool_.begin();
        const typename Pool::iterator end = pool_.end();
        for (; pos != end; ++pos) {
            delete[] (*pos);
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
