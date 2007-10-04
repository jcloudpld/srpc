#ifndef NSRPC_LOGMANAGER_H
#define NSRPC_LOGMANAGER_H

#include "../config/config.h"
#include <srpc/StringTypes.h>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <iosfwd>

class ACE_Log_Msg_Callback;

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * @class LogEventHandler
 * log message event handler
 */
class LogEventHandler
{
public:
    virtual ~LogEventHandler() {}

    virtual void onLog(const char* log) = 0;
};

/**
 * @class LogRepository
 */
class LogRepository
{
public:
    virtual ~LogRepository() {}

    virtual bool hasLog() const = 0;

    virtual srpc::String getLog() = 0;
private:
    friend class LogManager;
    virtual ACE_Log_Msg_Callback* getCallback() = 0;
protected:
    LogRepository() {}
};


/**
 * @struct LogRepositoryFactory
 * LogRepository Factory class.
 */
struct NSRPC_API LogRepositoryFactory
{
    static LogRepository* create(bool isThreadSafe,
        LogEventHandler* logEventHandler = 0);
};


/**
* @class LogManager
*  - ACE Log Manager
*/ 
class NSRPC_API LogManager : public boost::noncopyable
{
public:
    LogManager();
    ~LogManager();

    /// disable thelog
    static void silent();

    /// enable the log
    static void unsilent();

    /// log all message
    static void verbose();

    /// log all message except debug message
    static void noDebug();

    static void setMask(unsigned int masks);

    static void redirectToOStream(std::ostream* output,
        bool streamOnly = true);

    void redirectToFile(const srpc::String& filename, bool ostreamOnly = true);

    static void redirectToStderr(bool stderrOnly = true);

    static void redirectToSyslog(const char* progName);

    /// capture logging
    static void redirectToCallback(LogRepository& repository,
        bool callbackOnly = true);

    static bool isEnabledMask(unsigned int mask);
private:
    boost::scoped_ptr<std::ofstream> log_stream_;
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_LOGMANAGER_H
