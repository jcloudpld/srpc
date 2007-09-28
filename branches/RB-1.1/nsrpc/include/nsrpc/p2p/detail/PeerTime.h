#ifndef NSRPC_PEERTIME_H
#define NSRPC_PEERTIME_H

#include "../../config/config.h"
#include <srpc/Types.h>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/// ��� �ð� (msec ����)
typedef srpc::UInt32 PeerTime;

namespace detail
{

/**
 * ��� �ð��� ���Ѵ�(= ���� �ð� - ��� �ð�).
 * - ACE_Time_Value::msec()�� overflow�� �����ϱ� ���Ͽ� ��� �ð��� �̿�
 */
PeerTime NSRPC_API getPeerTime();

/// ��� �ð��� ���� �ð����� �ʱ�ȭ�Ѵ�.
void NSRPC_API resetBaseTime();


inline PeerTime getElapsedTime(PeerTime now, PeerTime past)
{
    return (now > past) ? (now - past) : 0;
}

} // namespace detail

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERTIME_H
