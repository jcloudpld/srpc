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
 * - call()�� ȣ���� �� �ݵ�� release()�� ȣ���ؾ� �Ѵ�. �׷��� ���� ���
 *   memory leak�� �߻��Ѵ�.
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
