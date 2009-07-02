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
 * ��Ʈ�� ���� ����
 */
class NSRPC_API Session
{
public:
    virtual ~Session() {}

    /// ���� ������ ���(����)�Ѵ�
    virtual void cancelConnection() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSION_H
