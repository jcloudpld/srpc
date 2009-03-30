#ifndef NSRPC_PEERTIME_H
#define NSRPC_PEERTIME_H

#include "../../nsrpc.h"
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
 * P2P�� ��� �ð�(= ���� �ð� - ��� �ð�)�� �����Ѵ�.
 * - getPeerTime()�� ���� ���� �� �ִ�
 * - ACE_Time_Value::msec()�� overflow�� �����ϱ� ���Ͽ� ��� �ð��� �̿�
 * @internal
 */
void NSRPC_API setPeerTime();


/**
 * ��� �ð��� ���Ѵ�.
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
