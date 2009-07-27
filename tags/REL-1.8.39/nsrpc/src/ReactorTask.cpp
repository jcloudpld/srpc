#include "stdafx.h"
#include <nsrpc/ReactorTask.h>
#include <nsrpc/utility/AceUtil.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4355 4800)
#endif
#include <ace/Task.h>
#include <ace/Reactor.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif


namespace nsrpc
{

namespace detail
{

/**
 * @class ReactorTaskImpl
 */
class ReactorTaskImpl : public ACE_Task_Base
{
public:
    ReactorTaskImpl(ACE_Reactor& reactor) :
        reactor_(reactor) {}

    bool start(size_t reactorThreadCount) {
        const long thread_flag = THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED;
        return activate(thread_flag,
            static_cast<int>(reactorThreadCount)) == 0;
    }

    void stop() {
        (void)reactor_.end_reactor_event_loop();
    }
private:
    virtual int svc() {
        disableSignals();
        reactor_.owner(ACE_OS::thr_self());
        return reactor_.run_reactor_event_loop();
    }
private:
    ACE_Reactor& reactor_;
};

} // namespace detail

// = ReactorTask

ReactorTask::ReactorTask(bool allocReactor) :
    allocReactor_(allocReactor),
    reactor_(allocReactor ? new ACE_Reactor : ACE_Reactor::instance()),
    pimpl_(new detail::ReactorTaskImpl(*reactor_))
{
}


ReactorTask::~ReactorTask()
{
    stop();
    if (allocReactor_) {
        boost::checked_delete(reactor_);
    }
}


bool ReactorTask::start(size_t reactorThreadCount)
{
    return pimpl_->start(reactorThreadCount);
}


void ReactorTask::stop()
{
    pimpl_->stop();
}


void ReactorTask::wait()
{
    pimpl_->wait();
}

} // namespace nsrpc
