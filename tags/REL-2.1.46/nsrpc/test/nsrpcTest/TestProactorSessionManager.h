#ifndef NSRPC_TESTPROACTORSESSIONMANAGER_H
#define NSRPC_TESTPROACTORSESSIONMANAGER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>
#include <nsrpc/config/Proactor.h>

#if defined(NSRPC_HAS_PROACTOR)

#include "TestProactorSession.h"
#include <nsrpc/SessionConfig.h>
#include <nsrpc/detail/SessionManager.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <nsrpc/utility/AceUtil.h>
#include <vector>

/**
 * @class TestProactorSessionManager
 *
 * 테스트용 SessionManager
 * - 메모리 누수가 발생하지만 의도적으로 놔둔 것임. 중요한 것은 세션 생성 여부!
 */
class TestProactorSessionManager : public nsrpc::SessionManager
{
public:
    TestProactorSessionManager(NSRPC_Proactor* proactor) :
        proactor_(proactor),
        lastSession_(0),
        sessionCount_(0) {
        messageBlockManager_.reset(
            new nsrpc::SynchMessageBlockManager(10, 1024));
    }

    virtual ~TestProactorSessionManager() {
    }

    virtual void initialize() {}

    virtual nsrpc::Session* acquire() {
        ++sessionCount_;
        nsrpc::SessionConfig config(this, messageBlockManager_.get(),
            nsrpc::PacketCoderFactory().create(), proactor_,
            nsrpc::SessionCapacity::getNoLimitedCapacity());
        lastSession_ = new TestProactorSession(config);
        return lastSession_;
    }

    virtual void release(nsrpc::Session* session) {
        session->closeConnection();
        --sessionCount_;
    }

    virtual void cancel() {}

    virtual void wait() {}

    virtual bool isSafeToDelete() const {
        return true;
    }

    virtual size_t getSessionCount() const {
        return sessionCount_;
    }
public:
    TestProactorSession& getSession() const {
        return *lastSession_;
    }

private:
    NSRPC_Proactor* proactor_;
    boost::scoped_ptr<nsrpc::SynchMessageBlockManager> messageBlockManager_;
    TestProactorSession* lastSession_;
    int sessionCount_;
};

#endif // #if defined(NSRPC_HAS_PROACTOR)

#endif // !defined(NSRPC_TESTPROACTORSESSIONMANAGER_H)
