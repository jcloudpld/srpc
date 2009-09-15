#ifndef NSRPC_SESSIONDESTROYER_H
#define NSRPC_SESSIONDESTROYER_H

#ifdef _MSC_VER
#  pragma once
#endif

namespace nsrpc
{

class Session;

/** @addtogroup session
* @{
*/

/**
 * @class SessionDestroyer
 *
 * Session 인스턴스를 해제한다.
 */
class SessionDestroyer
{
public:
    virtual ~SessionDestroyer() {}

    /// Session 인스턴스를 사용 후 해제한다.
    virtual void release(Session* session) = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONDESTROYER_H
