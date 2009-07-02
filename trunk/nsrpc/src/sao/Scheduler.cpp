#include "stdafx.h"
#include "Scheduler.h"
#include <nsrpc/sao/ProcessorCallback.h>
#include <nsrpc/sao/detail/MethodRequest.h>
#include <nsrpc/utility/Logger.h>
#include <nsrpc/utility/SystemUtil.h>

namespace nsrpc
{

namespace sao
{

Scheduler::~Scheduler()
{
    stop();

    flush();
}


bool Scheduler::start()
{
    assert(! stop_);
    stop_ = false;

    const int onlyOneThread = 1;
    const long flags = THR_NEW_LWP | THR_JOINABLE |THR_INHERIT_SCHED;
    return activate(flags, onlyOneThread) == 0;
}


void Scheduler::stop()
{
    {
        ACE_GUARD(ACE_Thread_Mutex, monitor, lock_);

        stop_ = true;
        available_.signal();
    }

    wait();
}


void Scheduler::schedule(MethodRequest* request)
{
    int queueSize = -1;
    {
        ACE_GUARD(ACE_Thread_Mutex, monitor, lock_);

        requestQueue_.push(request);
        available_.signal();

        queueSize = getRequestQueueSize();

        if ((queueSize > 0) && (callback_ != 0)) {
            callback_->scheduled(queueSize);
        }
    }

    if (queueSize >= 0) {
        NSRPC_LOG_INFO2("Scheduler: current queue size %u", queueSize);
    }
}


void Scheduler::flush()
{
    ACE_GUARD(ACE_Thread_Mutex, monitor, lock_);

    for (;;) {
        MethodRequest* request = getRpcMethod_i();
        if (! request) {
            break;
        }
        request->release();
    }
}


MethodRequest* Scheduler::getRpcMethod()
{
    ACE_GUARD_RETURN(ACE_Thread_Mutex, monitor, lock_, 0);

    while (requestQueue_.empty()) {
        if (stop_) {
            return 0;
        }
        available_.wait();
    }
    
    return getRpcMethod_i();
}


MethodRequest* Scheduler::getRpcMethod_i()
{
    if (requestQueue_.empty()) {
        return 0;
    }

    MethodRequest* method = requestQueue_.front();
    requestQueue_.pop();
    return method;
}


int Scheduler::getRequestQueueSize()
{
    const time_t logInterval = 5;
    const size_t queueThreshold = 5;

    const time_t currentTime = time(0);
    if ((currentTime - lastLogTime_) >= logInterval) {
        const size_t currentQueueSize = requestQueue_.size();
        const size_t queueSizeDiff =
            (currentQueueSize > prevQueueSize_) ?
                currentQueueSize - prevQueueSize_ :
                prevQueueSize_ - currentQueueSize;
        if (queueSizeDiff > queueThreshold) {
            lastLogTime_ = currentTime;
            prevQueueSize_ = currentQueueSize;
            return static_cast<int>(currentQueueSize);
        }
    }
    return -1;
}


int Scheduler::svc()
{
    while (! stop_) {
        MethodRequest* request = getRpcMethod();
        if (request != 0) {
            request->call();
        }
    }

    return 0;
}

} // namespace sao

} // namespace nsrpc
