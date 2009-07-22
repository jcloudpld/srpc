#ifndef NSRPC_SESSIONMANAGER_H
#define NSRPC_SESSIONMANAGER_H

#include "SessionCreator.h"
#include "SessionDestroyer.h"
#include <boost/noncopyable.hpp>

namespace nsrpc
{

/** @addtogroup session
* @{
*/


/**
 * @class SessionManagerCallback
 */
class SessionManagerCallback : public boost::noncopyable
{
public:
    virtual ~SessionManagerCallback() {}

    virtual void sessionAcquired(size_t acquiredSessionCount) = 0;

    virtual void sessionReleased(size_t acquiredSessionCount) = 0;
};


/**
 * @class SessionManager
 *
 * Asynchronous Session Manager
 */
class SessionManager :
    public SessionCreator, public SessionDestroyer, public boost::noncopyable
{
public:
    virtual ~SessionManager() {}

    /// SessionManager�� �ʱ�ȭ�Ѵ�. ��2�� ctor.
    virtual void initialize() = 0;

    /// ���� ��� ���� ��� ������ AIO�� ����ϰ� ���� ����(acquire)�� ���ϰ�
    /// ���´�.
    virtual void cancel() = 0;

    /// �����ϰ� ������ �� ���� ������ ����մϴ�.
    virtual void wait() = 0;

    /// �����ϰ� �޸𸮸� ������ �� �ִ°�?
    virtual bool isSafeToDelete() const = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONMANAGER_H
