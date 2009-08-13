#ifndef NSRPC_SESSION_H
#define NSRPC_SESSION_H

#ifdef _MSC_VER
#  pragma once
#endif

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
    virtual void closeConnection() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSION_H
