#ifndef NSRPC_PEERTIME_H
#define NSRPC_PEERTIME_H

#include "../../nsrpc.h"
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
 * P2P의 상대 시간(= 현재 시간 - 기반 시간)을 설정한다.
 * - getPeerTime()을 통해 얻을 수 있다
 * - ACE_Time_Value::msec()의 overflow를 방지하기 위하여 상대 시간을 이용
 * @internal
 */
void NSRPC_API setPeerTime();


/**
 * 상대 시간을 구한다.
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
