#include "stdafx.h"
#include <nsrpc/utility/LogManager.h>
#include <srpc/ContainerTypes.h>
#include <ace/Log_Msg_Callback.h>
#include <ace/Null_Mutex.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4244)
#endif
#include <ace/Log_Record.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <ace/Lock_Adapter_T.h>
#include <ace/OS.h>
#include <fstream>

namespace nsrpc
{

namespace
{

// = LogRepositoryImpl

/**
 * @class LogRepositoryImpl
 */
class LogRepositoryImpl : public LogRepository, public ACE_Log_Msg_Callback
{
    typedef srpc::Queue<srpc::String> Logs;
public:
    LogRepositoryImpl(bool isThreadSafe, LogEventHandler* logEventHandler) :
        logEventHandler_(logEventHandler) {
        if (isThreadSafe) {
            lock_.reset(new ACE_Lock_Adapter<ACE_Thread_Mutex>);
        }
        else {
            lock_.reset(new ACE_Lock_Adapter<ACE_Null_Mutex>);
        }
    }
private:
    // = LogRepository overriding
    virtual bool hasLog() const {
        if (logEventHandler_ != 0) {
            return 0;
        }
        else {
            ACE_GUARD_RETURN(ACE_Lock, monitor, *lock_, false);
            return hasLog_i();
        }
    }

    srpc::String getLog() {
        srpc::String log;
        if (! logEventHandler_) {
            ACE_GUARD_RETURN(ACE_Lock, monitor, *lock_, "");
            if (hasLog_i()) {
                log = logs_.front();
                logs_.pop();
            }
        }
        return log;
    }

    virtual ACE_Log_Msg_Callback* getCallback() {
        return this;
    }
private:
    // = ACE_Log_Msg_Callback overriding
    virtual void log(ACE_Log_Record& logRecord) {
        const char* log = logRecord.msg_data();
        if (logEventHandler_ != 0) {
            logEventHandler_->onLog(log);
        }
        else {
            ACE_GUARD(ACE_Lock, monitor, *lock_);
            logs_.push(log);
        }
    }
private:
    bool hasLog_i() const {
        return ! logs_.empty();
    }
private:
    LogEventHandler* logEventHandler_;
    Logs logs_;

    mutable boost::scoped_ptr<ACE_Lock> lock_;
};

} // namespace

// = LogRepositoryFactory

LogRepository* LogRepositoryFactory::create(bool isThreadSafe,
    LogEventHandler* logEventHandler)
{
    return new LogRepositoryImpl(isThreadSafe, logEventHandler);
}

// = LogManager

LogManager::LogManager() :
    log_stream_(0)
{
}


LogManager::~LogManager()
{
    if (log_stream_.get() != 0) {
        log_stream_->close();
    }
}


void LogManager::setMask(unsigned int masks)
{
    ACE_LOG_MSG->priority_mask(masks, ACE_Log_Msg::PROCESS);
}


void LogManager::silent()
{
    ACE_LOG_MSG->set_flags(ACE_Log_Msg::SILENT);
}


void LogManager::unsilent()
{
    ACE_LOG_MSG->clr_flags(ACE_Log_Msg::SILENT);
}


void LogManager::verbose()
{
    setMask(LM_DEBUG | LM_NOTICE | LM_TRACE | LM_INFO | LM_ERROR);
}


void LogManager::noDebug()
{
    setMask(LM_NOTICE | LM_INFO | LM_ERROR);
}


void LogManager::redirectToOStream(std::ostream* output,
    bool streamOnly)
{
    ACE_LOG_MSG->msg_ostream(output);

    if (streamOnly) {
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER |
            ACE_Log_Msg::MSG_CALLBACK);
    }

    ACE_LOG_MSG->set_flags(ACE_Log_Msg::OSTREAM);    
}


void LogManager::redirectToFile(const srpc::String& filename, 
    bool ostreamOnly)
{
    log_stream_.reset(new std::ofstream);
    log_stream_->open(filename.c_str(), ios::out | ios::app);
    redirectToOStream(log_stream_.get(), ostreamOnly);
}


void LogManager::redirectToStderr(bool stderrOnly)
{
    if (! stderrOnly) {
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::OSTREAM | ACE_Log_Msg::LOGGER |
            ACE_Log_Msg::MSG_CALLBACK);
    }
    ACE_LOG_MSG->set_flags(ACE_Log_Msg::STDERR);
}


void LogManager::redirectToSyslog(const char* progName)
{
    ACE_LOG_MSG->open(progName, ACE_Log_Msg::SYSLOG, progName);
}


void LogManager::redirectToCallback(
    LogRepository& repository, bool callbackOnly)
{
    (void)ACE_LOG_MSG->msg_callback(repository.getCallback());

    ACE_LOG_MSG->set_flags(ACE_Log_Msg::MSG_CALLBACK);

    if (callbackOnly) {
        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::OSTREAM | ACE_Log_Msg::LOGGER |
            ACE_Log_Msg::STDERR);
    }
}


bool LogManager::isEnabledMask(unsigned int mask)
{
    return ACE_BIT_DISABLED(ACE_LOG_MSG->flags(), ACE_Log_Msg::SILENT) &&
        ACE_LOG_MSG->log_priority_enabled(static_cast<ACE_Log_Priority>(mask));
}

} // namespace nsrpc
