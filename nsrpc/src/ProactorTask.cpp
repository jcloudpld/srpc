#include "stdafx.h"

#if defined (NSRPC_HAS_PROACTOR)

#include <nsrpc/ProactorTask.h>
#include <nsrpc/utility/AceUtil.h>
#if defined (NSRPC_USE_TPROACTOR)
#  include <TProactor/Proactor.h>
#endif

namespace nsrpc
{

// = ProactorTask

ProactorTask::ProactorTask(std::auto_ptr<NSRPC_Proactor> proactor,
    bool deleteProactor) :
    proactor_(proactor.release()),
    deleteProactor_(deleteProactor)
{
    if (! proactor_) {
        proactor_ = NSRPC_Proactor::instance();
        deleteProactor_ = false;
    }
}


ProactorTask::~ProactorTask()
{
    stop();
    if (deleteProactor_) {
        boost::checked_delete(proactor_);
    }
}


bool ProactorTask::start(size_t proactorThreadCount)
{
    const long thread_flag = THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED;
    return activate(thread_flag,
        static_cast<int>(proactorThreadCount)) == 0;
}


void ProactorTask::stop()
{
    (void)proactor_->proactor_end_event_loop();
}


int ProactorTask::svc()
{
    disableSignals();

    return proactor_->proactor_run_event_loop();
}

} // namespace nsrpc

#endif // #if defined (NSRPC_HAS_PROACTOR)
