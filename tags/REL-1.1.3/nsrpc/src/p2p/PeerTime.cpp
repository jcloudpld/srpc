#include "stdafx.h"
#include <nsrpc/p2p/detail/PeerTime.h>
#include <ace/High_Res_Timer.h>

namespace nsrpc
{

namespace detail
{

namespace
{

static ACE_Time_Value baseTime(ACE_High_Res_Timer::gettimeofday_hr());

} // namespace

PeerTime getPeerTime()
{
    return (ACE_High_Res_Timer::gettimeofday_hr() - baseTime).msec();
}


void resetBaseTime()
{
    baseTime = ACE_High_Res_Timer::gettimeofday_hr();
}

} // namespace detail

} // namespace nsrpc
