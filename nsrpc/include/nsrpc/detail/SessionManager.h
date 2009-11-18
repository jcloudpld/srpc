#ifndef NSRPC_SESSIONMANAGER_H
#define NSRPC_SESSIONMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

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

    /// @return is allowed to acquire more?
    virtual bool sessionAcquired(size_t acquiredSessionCount) = 0;

    /// @return is allowed to acquire more?
    virtual bool sessionReleased(size_t acquiredSessionCount) = 0;
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

    /// ���Ǽ��� ��´�
    virtual size_t getSessionCount() const = 0;

    /// ��ü ���Ǽ�(Ǯ)�� ��´�
    virtual size_t getTotalSessionCount() const = 0;

};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONMANAGER_H
