#include "stdafx.h"

#if defined (NSRPC_HAS_PROACTOR)

#include <nsrpc/Session.h>
#include <nsrpc/CachedSessionManager.h>
#include <nsrpc/SessionFactory.h>
#include <nsrpc/detail/SessionPool.h>
#include <nsrpc/utility/AceUtil.h>
#include <nsrpc/utility/SystemUtil.h>
#include <nsrpc/utility/Logger.h>

namespace nsrpc
{

/**
* @class SessionAllocator
*
* CachedSessionManager와 SessionPool을 연결하기 위한 helper class.
*/
class SessionAllocator
{
public:
    SessionAllocator(SessionFactory& sessionFactory,
        SessionDestroyer& sessionDestroyer) :
        sessionFactory_(sessionFactory) {
        sessionFactory_.setSessionDestroyer(sessionDestroyer);
    }

    Session* malloc() {
        return sessionFactory_.create();
    }

    void free(Session* session) {
        assert(session != 0);
        boost::checked_delete(session);
    }
private:
    SessionFactory& sessionFactory_;
};

// = CachedSessionManager

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

// poolSize의 10%를 여분으로 둠
CachedSessionManager::CachedSessionManager(const srpc::String& name,
    size_t poolSize, SessionFactory& sessionFactory,
    SessionManagerCallback* callback) :
    sessionAllocator_(new SessionAllocator(sessionFactory, *this)),
    sessionPool_(
        new CachedSessionPool(poolSize, poolSize / 10, *sessionAllocator_)),
    name_(name),
    callback_(callback),
    enabled_(false)
{
}

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

CachedSessionManager::~CachedSessionManager()
{
}


void CachedSessionManager::initialize()
{
    sessionPool_->initialize();

    enabled_ = true;
}


Session* CachedSessionManager::acquire()
{
    if (! enabled_) {
        return 0;
    }

    Session* session = sessionPool_->acquire();
    if (! session) {
        return 0;
    }

    const size_t sessionCount = sessionPool_->getActiveResourceCount();
    NSRPC_LOG_DEBUG3("CachedSessionManager(%s) - %d session(s) acquired.",
        name_.c_str(), sessionCount);

    if (callback_ != 0) {
        const bool prevState = enabled_;
        enabled_ = callback_->sessionAcquired(sessionCount);
        if ((! enabled_) && prevState) {
            NSRPC_LOG_DEBUG2("CachedSessionManager(%s) - disabled.",
                name_.c_str());
        }
    }
    return session;
}


void CachedSessionManager::release(Session* session)
{
    assert(session != 0);

    sessionPool_->release(session);

    const size_t sessionCount = sessionPool_->getActiveResourceCount();
    NSRPC_LOG_DEBUG3("CachedSessionManager(%s) - %d session(s) left.",
        name_.c_str(), sessionCount);

    if (callback_ != 0) {
        const bool prevState = enabled_;
        enabled_ = callback_->sessionReleased(sessionCount);
        if ((! prevState) && enabled_) {
            NSRPC_LOG_DEBUG2("CachedSessionManager(%s) - enabled.",
                name_.c_str());
        }
    }
}


void CachedSessionManager::cancel()
{
    enabled_ = false;
    sessionPool_->cancel();
}


void CachedSessionManager::wait()
{
    while (! isSafeToDelete()) {
        pause(1);
    }
}


size_t CachedSessionManager::getActiveSessionCount() const
{
    return sessionPool_->getActiveResourceCount();
}


size_t CachedSessionManager::getInactiveSessionCount() const
{
    return sessionPool_->getInactiveResourceCount();
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
