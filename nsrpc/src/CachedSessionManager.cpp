#include "stdafx.h"
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
    SessionAllocator(const SessionFactory& sessionFactory,
        SessionDestroyer& sessionDestroyer) :
        sessionFactory_(sessionFactory),
        sessionDestroyer_(sessionDestroyer) {}

    Session* malloc() {
        return sessionFactory_.create(sessionDestroyer_);
    }

    void free(Session* session) {
        assert(session != 0);
        boost::checked_delete(session);
    }
private:
    const SessionFactory& sessionFactory_;
    SessionDestroyer& sessionDestroyer_;
};

// = CachedSessionManager

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4355)
#endif

// poolSize의 10%를 여분으로 둠
CachedSessionManager::CachedSessionManager(const std::string& name,
    size_t poolSize, const SessionFactory& sessionFactory) :
    sessionAllocator_(new SessionAllocator(sessionFactory, *this)),
    sessionPool_(
        new CachedSessionPool(poolSize, poolSize / 10, *sessionAllocator_)),
    name_(name),
    shouldFinish_(false)
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
}


Session* CachedSessionManager::acquire()
{
    if (! shouldFinish_) {
        Session* session = sessionPool_->acquire();
        NSRPC_LOG_DEBUG3("CachedSessionManager(%s) - %d session(s) acquired.",
               name_.c_str(), sessionPool_->getActiveResourceCount());
        return session;
    }
    return 0;
}


void CachedSessionManager::release(Session* session)
{
    assert(session != 0);

    sessionPool_->release(session);

    NSRPC_LOG_DEBUG3("CachedSessionManager(%s) - %d session(s) left.",
        name_.c_str(), sessionPool_->getActiveResourceCount());
}


void CachedSessionManager::cancel()
{
    shouldFinish_ = true;
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
