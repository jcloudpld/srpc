#ifndef NSRPC_TESTSESSIONMANAGER_H
#define NSRPC_TESTSESSIONMANAGER_H

#include "TestSession.h"
#include <nsrpc/SessionConfig.h>
#include <nsrpc/detail/SessionManager.h>
#include <nsrpc/utility/MessageBlockManager.h>
#include <nsrpc/utility/AceUtil.h>
#include <vector>

/**
 * @class TestSessionManager
 *
 * �׽�Ʈ�� SessionManager
 * - �޸� ������ �߻������� �ǵ������� ���� ����. �߿��� ���� ���� ���� ����!
 */
class TestSessionManager : public nsrpc::SessionManager
{
public:
    TestSessionManager(NSRPC_Proactor* proactor) :
        proactor_(proactor),
        lastSession_(0),
        sessionCount_(0) {
        messageBlockManager_.reset(
            new nsrpc::SynchMessageBlockManager(10, 1024));
    }

    virtual ~TestSessionManager() {
    }

    virtual void initialize() {}

    virtual nsrpc::Session* acquire() {
        ++sessionCount_;
        nsrpc::SessionConfig config(this, messageBlockManager_.get(),
            nsrpc::PacketCoderFactory().create(), proactor_,
            nsrpc::SessionCapacity::getNoLimitedCapacity());
        lastSession_ = new TestSession(config);
        return lastSession_;
    }

    virtual void release(nsrpc::Session* session) {
        session->disconnect();
        --sessionCount_;
    }

    virtual void cancel() {}

    virtual void wait() {}

    virtual bool isSafeToDelete() const {
        return true;
    }
public:
    TestSession& getSession() const {
        return *lastSession_;
    }

    int getSessionCount() const {
        return sessionCount_;
    }
private:
    NSRPC_Proactor* proactor_;
    boost::scoped_ptr<nsrpc::SynchMessageBlockManager> messageBlockManager_;
    TestSession* lastSession_;
    int sessionCount_;
};

#endif // !defined(NSRPC_TESTSESSIONMANAGER_H)
