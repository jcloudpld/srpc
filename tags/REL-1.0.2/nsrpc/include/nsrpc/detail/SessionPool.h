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
    /// TODO: 좀 더 깔끔하게 처리할 방법 없나?
    void cancel() {
        // dead-lock을 피하기 위해 Session::disconnect() 호출을 따로 빼냄.
        typename Parent::ActiveResources activeResources;
        {
            ACE_GUARD(Mutex, monitor, this->getLock());
            activeResources = this->getActiveResources();
        }

        std::for_each(activeResources.begin(),
            activeResources.end(), std::mem_fun(&Session::disconnect));
    }
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONPOOL_H
