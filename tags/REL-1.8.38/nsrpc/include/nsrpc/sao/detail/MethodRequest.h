#ifndef NSRPC_SAO_METHODREQUEST_H
#define NSRPC_SAO_METHODREQUEST_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "MethodReleaser.h"
#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace sao
{

/**
 * @class MethodRequest
 *
 * AO method base class
 * - Command Pattern
 * - call()을 호출한 후 반드시 release()를 호출해야 한다. 그렇지 않을 경우
 *   memory leak이 발생한다.
 */
class MethodRequest : public boost::noncopyable
{
public:
    explicit MethodRequest(MethodReleaser* releaser = 0) :
        releaser_(releaser) {}

    virtual ~MethodRequest() {}

    virtual void call() = 0;

    virtual void release() {
        if (releaser_ != 0) {
            releaser_->release(this);
        }
    }
private:
    MethodReleaser* releaser_;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_METHODREQUEST_H
