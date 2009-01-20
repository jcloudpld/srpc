#ifndef NSRPC_SESSIONDESTROYER_H
#define NSRPC_SESSIONDESTROYER_H

namespace nsrpc
{

class Session;

/** @addtogroup session
* @{
*/

/**
 * @class SessionDestroyer
 *
 * Session �ν��Ͻ��� �����Ѵ�.
 */
class SessionDestroyer
{
public:
    virtual ~SessionDestroyer() {}

    /// Session �ν��Ͻ��� ��� �� �����Ѵ�.
    virtual void release(Session* session) = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONDESTROYER_H
