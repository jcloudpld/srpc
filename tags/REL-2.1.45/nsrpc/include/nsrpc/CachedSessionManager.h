#ifndef NSRPC_CACHEDSESSIONMANAGER_H
#define NSRPC_CACHEDSESSIONMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "nsrpc.h"
#include "config/Proactor.h"
#include "utility/AceUtil.h"

#if defined (NSRPC_HAS_PROACTOR)

#include "detail/SessionManager.h"
#include <srpc/StringTypes.h>
#include <boost/scoped_ptr.hpp>

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
 * - Session �ν��Ͻ��� ������ �����ϹǷ� ���ο� ������ ���۵� ���
 *   Session::open()�� �������̵��Ͽ� �����ϰ� �ʱ�ȭ�� ���־�� �Ѵ�.
 *   
 */
class NSRPC_API CachedSessionManager : public SessionManager
{
    typedef SessionPool<SessionAllocator, Thread_Mutex_With_SpinLock>
        CachedSessionPool;
public:
    /**
     * ctor
     * @param name ���� �̸�. ����� �뵵�� ���.
     * @param poolSize Session Pool ũ��. ���� �� ��� poolSize��ŭ ������ ����
     *                 �Ѵ�.
     * @param sessionFactory ���ο� ������ ���� ���丮 �ν��Ͻ�.
     */
    CachedSessionManager(const srpc::String& name, size_t poolSize,
        SessionFactory& sessionFactory, SessionManagerCallback* callback = 0);

    virtual ~CachedSessionManager();

    /// ��� ���� ���� ���� ��´�.
    size_t getActiveSessionCount() const;

    /// ��� ���� ���� ���� ��´�.
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
    virtual size_t getSessionCount() const {
        return getActiveSessionCount();
    }

private:
    /// �� ���� �Ŵ������� ������ �޼��� �� �Ŵ���.
    boost::scoped_ptr<SessionAllocator> sessionAllocator_;
    boost::scoped_ptr<CachedSessionPool> sessionPool_;
    srpc::String name_;
    SessionManagerCallback* callback_;

    bool shouldFinish_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)

#endif // NSRPC_CACHEDSESSIONMANAGER_H
