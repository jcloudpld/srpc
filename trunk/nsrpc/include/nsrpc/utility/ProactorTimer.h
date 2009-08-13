#ifndef NSRPC_PROACTORTIMER_H
#define NSRPC_PROACTORTIMER_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"
#include "../config/Proactor.h"
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

class NSRPC_Proactor;

namespace nsrpc
{

namespace
{
class ProactorTimerImpl;
} // namespace

/** @addtogroup utility
* @{
*/

/**
 * @class ProactorTimerCallback
 */
class NSRPC_API ProactorTimerCallback : public boost::noncopyable
{
public:
    virtual ~ProactorTimerCallback() {}

    virtual void timerFired() = 0;
};

/**
 * @class ProactorTimer
 * Proactor 기반의 Timer 클래스
 * - PIMPL idiom
 */
class NSRPC_API ProactorTimer : public boost::noncopyable
{
public:
    ProactorTimer(ProactorTimerCallback& callback,
        NSRPC_Proactor* proactor = NSRPC_Proactor::instance());
    virtual ~ProactorTimer();

    bool setup(size_t msec);

    long getId() const;

private:
    boost::scoped_ptr<ProactorTimerImpl> impl_;
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_PROACTORTIMER_H
