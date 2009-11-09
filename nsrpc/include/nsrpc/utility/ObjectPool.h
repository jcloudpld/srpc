#ifndef NSRPC_OBJECTPOOL_H
#define NSRPC_OBJECTPOOL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "MemoryPool.h"

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * @class ObjectPool
 *
 * Resource pool
 * - Resouce: ObjectPool���� �ٷ� ������ Ÿ��.
 * - Allocator: Resource �ν��Ͻ��� �Ҵ�/�����Ѵ�.
 * - Mutex: Mutex Ÿ��(��, ACE_Null_Mutex, ACE_Thread_Mutex, ...)
 */
template <typename Resource, class Allocator, class Mutex = ACE_Null_Mutex>
class ObjectPool :
    public MemoryPool<sizeof(Resource), Resource, Allocator, Mutex>
{
public:
    ObjectPool(size_t poolSize, Allocator& allocator, bool growable = true,
        size_t spareSize = 0, MemoryPoolCallback* callback = 0) :
        MemoryPool<sizeof(Resource), Resource, Allocator, Mutex>(
            poolSize, allocator, growable, spareSize, callback) {}
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_OBJECTPOOL_H
