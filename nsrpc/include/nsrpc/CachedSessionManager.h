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
 * - Session �ν��Ͻ��� ������ �����ϹǷ� ���ο� ������ ���۵� ���
 *   Session::open()�� �������̵��Ͽ� �����ϰ� �ʱ�ȭ�� ���־�� �Ѵ�.
 *   
 */
class NSRPC_API CachedSessionManager : public SessionManager
{
    typedef SessionPool<SessionAllocator, ACE_Thread_Mutex> CachedSessionPool;
public:
    /**
     * ctor
     * @param name ���� �̸�. ����� �뵵�� ���.
     * @param poolSize Session Pool ũ��. ���� �� ��� poolSize��ŭ ������ ����
     *                 �Ѵ�.
     * @param sessionFactory ���ο� ������ ���� ���丮 �ν��Ͻ�.
     */
    CachedSessionManager(const srpc::String& name, size_t poolSize,
        SessionFactory& sessionFactory);

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
private:
    /// �� ���� �Ŵ������� ������ �޼��� �� �Ŵ���.
    boost::scoped_ptr<SessionAllocator> sessionAllocator_;
    boost::scoped_ptr<CachedSessionPool> sessionPool_;
    srpc::String name_;
    
    bool shouldFinish_;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_CACHEDSESSIONMANAGER_H
