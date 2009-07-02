#ifndef NSRPC_SESSIONPOOL_H
#define NSRPC_SESSIONPOOL_H

#include "../Session.h"
#include "../utility/ObjectPool.h"
#include <algorithm>

namespace nsrpc
{

class Session;

/** @addtogroup session
* @{
*/

/**
 * @class SessionPool
 *
 * Session Pool
 */
template <class Allocator, class Mutex>
class SessionPool : public ObjectPool<Session, Allocator, Mutex>
{
    typedef ObjectPool<Session, Allocator, Mutex> Parent;
public:
    SessionPool(size_t poolSize, size_t spareSize,
        Allocator& sessionAllocator) :
        Parent(poolSize, sessionAllocator, true, spareSize) {}

    /// 현재 사용 중인 모든 세션을 취소한다.
    void cancel() {
        Parent::forEachActiveResources(std::mem_fun(&Session::cancelConnection));
    }
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONPOOL_H
