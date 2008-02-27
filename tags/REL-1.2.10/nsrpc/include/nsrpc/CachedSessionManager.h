#ifndef NSRPC_CACHEDSESSIONMANAGER_H
#define NSRPC_CACHEDSESSIONMANAGER_H

#include "config/config.h"
#include "config/Proactor.h"
#include "detail/SessionManager.h"
#include <srpc/StringTypes.h>
#include <boost/scoped_ptr.hpp>

class NSRPC_Proactor;

namespace nsrpc
{

class SessionAllocator;
class SessionFactory;
template <class Allocator, class Mutex> class SessionPool;

/** @addtogroup session
* @{
*/

/**
 * @class CachedSessionManager
 *
 * Cached(Pooling) Session Manager
 * - Session 인스턴스는 재사용이 가능하므로 새로운 세션이 시작될 경우
 *   Session::open()을 오버라이딩하여 적절하게 초기화를 해주어야 한다.
 *   
 */
class NSRPC_API CachedSessionManager : public SessionManager
{
    typedef SessionPool<SessionAllocator, ACE_Thread_Mutex> CachedSessionPool;
public:
    /**
     * ctor
     * @param name 고유 이름. 디버깅 용도로 사용.
     * @param poolSize Session Pool 크기. 가득 찰 경우 poolSize만큼 세션을 생성
     *                 한다.
     * @param sessionFactory 새로운 세션을 만들 팩토리 인스턴스.
     */
    CachedSessionManager(const srpc::String& name, size_t poolSize,
        SessionFactory& sessionFactory);

    virtual ~CachedSessionManager();

    /// 사용 중인 세션 수를 얻는다.
    size_t getActiveSessionCount() const;

    /// 대기 중인 세션 수를 얻는다.
    size_t getInactiveSessionCount() const;
protected: // for Test
    // = implementation
    virtual void initialize();
    virtual Session* acquire();
    virtual void release(Session* session);
    virtual void cancel();
    virtual void wait();
    virtual bool isSafeToDelete() const {
        return getActiveSessionCount() == 0;
    }
private:
    /// 이 세션 매니저에서 공유할 메세지 블럭 매니저.
    boost::scoped_ptr<SessionAllocator> sessionAllocator_;
    boost::scoped_ptr<CachedSessionPool> sessionPool_;
    srpc::String name_;
    
    bool shouldFinish_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_CACHEDSESSIONMANAGER_H
