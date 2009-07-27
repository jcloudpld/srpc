#ifndef NSRPC_SAO_PROCESSOR_H
#define NSRPC_SAO_PROCESSOR_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace sao
{

class ProcessorCallback;
class Scheduler;
class Proxy;

/**
 * @class Processor
 * 요청을 단일 쓰레드에서 처리하기 위한 시스템
 * - Active Object
 * - Facade Pattern
 * - Active Object Pattern
 */
class NSRPC_API Processor : public boost::noncopyable
{
public:
    Processor(ProcessorCallback* callback = 0);
    ~Processor();

    bool initialize();
    void finalize();

    Proxy& getProxy() {
        return *proxy_;
    }
private:
    boost::scoped_ptr<Scheduler> scheduler_; ///< AO scheduler
    boost::scoped_ptr<Proxy> proxy_; ///< AO proxy
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_PROCESSOR_H
