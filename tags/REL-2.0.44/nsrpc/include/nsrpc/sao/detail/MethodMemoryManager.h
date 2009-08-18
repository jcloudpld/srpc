#ifndef NSRPC_SAO_METHODMEMORYMANAGER_H
#define NSRPC_SAO_METHODMEMORYMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "MethodReleaser.h"
#include <nsrpc/utility/ObjectPool.h>
#include <boost/noncopyable.hpp>

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

namespace nsrpc
{

namespace sao
{

/**
 * @class MethodMemoryManager
 * AO RequestMethod의 메모리를 관리한다
 */
template <typename Method, typename Mutex = ACE_Null_Mutex>
class MethodMemoryManager : private MethodReleaser
{
    /**
    * @class MethodAllocator
    */
    template <typename Method>
    class MethodAllocator
    {
    public:
        MethodAllocator(MethodReleaser& releaser) :
            releaser_(releaser) {}

        Method* malloc() {
            return new Method(&releaser_);
        }

        void free(Method* method) {
            boost::checked_delete(method);
        }
    private:
        MethodReleaser& releaser_;
    };

    /**
    * @class RpcMethodPool
    */
    template <typename Method, typename Mutex>
    class MethodPool :
        public nsrpc::ObjectPool<Method, MethodAllocator<Method>, Mutex>
    {
    public:
        MethodPool(size_t poolSize, MethodAllocator<Method>& allocator) :
            nsrpc::ObjectPool<Method, MethodAllocator<Method>, Mutex>(
            poolSize, allocator) {}
    };
public:
    explicit MethodMemoryManager(size_t poolSize = 100) :
        allocator_(*this),
        pool_(poolSize, allocator_) {
        pool_.initialize();
    }

    Method* acquire() {
        return pool_.acquire();
    }
private:
    // = MethodReleaser overriding
    virtual void release(void* method) {
        pool_.release(static_cast<Method*>(method));
    }
private:
    MethodAllocator<Method> allocator_;
    MethodPool<Method, Mutex> pool_;
};

} // namespace sao

} // namespace nsrpc

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#endif // NSRPC_SAO_METHODMEMORYMANAGER_H
