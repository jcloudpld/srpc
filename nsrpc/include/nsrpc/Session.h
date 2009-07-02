#ifndef NSRPC_SESSION_H
#define NSRPC_SESSION_H

#include "nsrpc.h"

namespace nsrpc
{

/** @addtogroup session
* @{
*/

/**
 * @class Session
 *
 * 네트웍 연결 세션
 */
class NSRPC_API Session
{
public:
    virtual ~Session() {}

    /// 세션 연결을 취소(해제)한다
    virtual void closeConnection() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSION_H
