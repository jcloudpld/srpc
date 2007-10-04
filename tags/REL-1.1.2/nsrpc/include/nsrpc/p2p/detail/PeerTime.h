#ifndef NSRPC_PEERTIME_H
#define NSRPC_PEERTIME_H

#include "../../config/config.h"
#include <srpc/Types.h>

namespace nsrpc
{

/** @addtogroup p2p
* @{
*/

/// 상대 시간 (msec 단위)
typedef srpc::UInt32 PeerTime;

namespace detail
{

/**
 * 상대 시간을 구한다(= 현재 시간 - 기반 시간).
 * - ACE_Time_Value::msec()의 overflow를 방지하기 위하여 상대 시간을 이용
 */
PeerTime NSRPC_API getPeerTime();

/// 기반 시간을 현재 시간으로 초기화한다.
void NSRPC_API resetBaseTime();


inline PeerTime getElapsedTime(PeerTime now, PeerTime past)
{
    return (now > past) ? (now - past) : 0;
}

} // namespace detail

/** @} */ // addtogroup p2p

} // namespace nsrpc

#endif // NSRPC_PEERTIME_H
