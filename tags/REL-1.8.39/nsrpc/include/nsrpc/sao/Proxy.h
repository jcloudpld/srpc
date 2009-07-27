#ifndef NSRPC_SAO_PROXY_H
#define NSRPC_SAO_PROXY_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace sao
{

class Scheduler;
class MethodRequest;

/**
 * @class Proxy
 *
 * 클라이언트의 요청을 Scheduler에게 넘겨 주기 위한 PROXY
 * - Proxy Pattern
 * - Active Object Proxy
 */
class NSRPC_API Proxy : public boost::noncopyable
{
public:
    Proxy(Scheduler& scheduler)  :
        scheduler_(scheduler) {}

    void schedule(MethodRequest* method);

private:
    Scheduler& scheduler_;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_PROXY_H
