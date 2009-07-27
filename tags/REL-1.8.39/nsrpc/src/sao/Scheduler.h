#ifndef NSRPC_SAO_SCHEDULER_H
#define NSRPC_SAO_SCHEDULER_H

#pragma warning ( push )
#pragma warning ( disable: 4251 4800 4996 )
#include <ace/Task.h>
#pragma warning ( pop )
#include <queue>
#include <ctime>

namespace nsrpc
{

namespace sao
{

class MethodRequest;
class ProcessorCallback;

/**
 * @class Scheduler
 *
 * 클라이언트의 요청을 처리하기 위한 Task
 */
class Scheduler : private ACE_Task_Base
{
    typedef std::queue<MethodRequest*> RpcMethodQueue;
public:
    Scheduler(ProcessorCallback* callback) :
        callback_(callback),
        stop_(false),
        available_(lock_),
        lastLogTime_(time(0)),
        prevQueueSize_(0) {}

    virtual ~Scheduler();

    bool start();
    void stop();

    /// Method queue를 비운다
    void flush();

    void schedule(MethodRequest* request);

private:
    MethodRequest* getRpcMethod();
    MethodRequest* getRpcMethod_i();

    int getRequestQueueSize();
private:

    // = ACE_Task_Base overring
    virtual int svc();
private:
    ProcessorCallback* callback_;
    RpcMethodQueue requestQueue_;
    volatile bool stop_;

    ACE_Thread_Mutex lock_;
    ACE_Condition_Thread_Mutex available_;
    time_t lastLogTime_;
    size_t prevQueueSize_;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_SCHEDULER_H
