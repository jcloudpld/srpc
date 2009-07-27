#ifndef SVOIP_THREAD_H
#define SVOIP_THREAD_H

#include <ace/Thread.h>
#include <boost/noncopyable.hpp>

namespace svoip
{

/**
 * @class Task
 */
class Task : public boost::noncopyable
{
public:
    Task() :
        threadId_(0),
        shouldStop_(false) {}

    virtual ~Task() {
        stop();
    }

    bool start() {
        return ACE_Thread::spawn(&Task::threadFunc, this,
            THR_NEW_LWP | THR_JOINABLE, &threadId_) == 0;
    }

    void stop() {
        shouldStop_ = true;
        if (threadId_ != 0) {
            ACE_Thread::join(&threadId_);
        }
    }

protected:
    bool shouldStop() const {
        return shouldStop_;
    }

private:
    virtual int svc() = 0;

private:
    static ACE_THR_FUNC_RETURN threadFunc(void* arg) {
        return static_cast<Task*>(arg)->svc();
    }

private:
    ACE_thread_t threadId_;
    volatile bool shouldStop_;
};

} // namespace svoip

#endif // SVOIP_THREAD_H
