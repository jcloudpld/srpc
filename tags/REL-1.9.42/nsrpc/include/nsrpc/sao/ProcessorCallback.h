#ifndef NSRPC_SAO_PROCESSORCALLBACK_H
#define NSRPC_SAO_PROCESSORCALLBACK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace sao
{

/**
 * @class ProcessorCallback
 */
class ProcessorCallback : public boost::noncopyable
{
public:
    virtual ~ProcessorCallback() {}

    virtual void scheduled(size_t queueSize) = 0;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_PROCESSORCALLBACK_H
