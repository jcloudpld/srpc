#include "stdafx.h"
#include <nsrpc/p2p/detail/PeerTime.h>
#include <ace/High_Res_Timer.h>

namespace nsrpc
{

namespace detail
{

namespace
{

ACE_Time_Value baseTime(ACE_High_Res_Timer::gettimeofday_hr());

PeerTime currentPeerTime = 0;

} // namespace

void setPeerTime()
{
    currentPeerTime =
        (ACE_High_Res_Timer::gettimeofday_hr() - baseTime).msec();
}


PeerTime getPeerTime()
{
    return currentPeerTime;
}


void resetBaseTime()
{
    baseTime = ACE_High_Res_Timer::gettimeofday_hr();
}

} // namespace detail

} // namespace nsrpc
