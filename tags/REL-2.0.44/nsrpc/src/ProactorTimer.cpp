#include "stdafx.h"
#include <nsrpc/utility/ProactorTimer.h>
#include <nsrpc/utility/AceUtil.h>
#include <ace/Proactor.h>

namespace nsrpc
{

namespace
{

/**
 * @class ProactorTimerImpl
 */
class ProactorTimerImpl : private NSRPC_Handler
{
public:
    ProactorTimerImpl(ProactorTimerCallback& callback,
        NSRPC_Proactor* proactor = NSRPC_Proactor::instance()) :
        callback_(callback),
        proactor_(proactor),
        timerId_(-1) {}

    virtual ~ProactorTimerImpl() {
        if (timerId_ != -1) {
            cancelTimer(*proactor_, timerId_);
        }
    }

    bool setup(size_t msec) {
        assert(timerId_ == -1);

        timerId_ = setupTimer(*proactor_, *this, msec, &timerId_);
        return timerId_ != -1;
    }

    //bool setupRepeatingTimer(size_t msec) {
    //    assert(timerId_ == -1);

    //    timerId_ = setupRepeatingTimer(*proactor_, *this, msec, &timerId_);
    //    return timerId_ != -1;
    //}

    long getId() const {
        return timerId_;
    }

private:
    void handle_time_out(const ACE_Time_Value& /*tv*/, const void* act) {
        assert(act == &timerId_); act;
        callback_.timerFired();
    }

private:
    ProactorTimerCallback& callback_;
    NSRPC_Proactor* proactor_;
    long timerId_;
};

} // namespace

// = ProactorTimer

ProactorTimer::ProactorTimer(ProactorTimerCallback& callback,
    NSRPC_Proactor* proactor) :
    impl_(new ProactorTimerImpl(callback, proactor))
{
}


ProactorTimer::~ProactorTimer()
{
}


bool ProactorTimer::setup(size_t msec)
{
    return impl_->setup(msec);
}


long ProactorTimer::getId() const
{
    return impl_->getId();
}

} // namespace nsrpc
