#include "stdafx.h"
#include <nsrpc/utility/SystemUtil.h>
#include <nsrpc/utility/AceUtil.h>

namespace nsrpc
{

void pause(size_t msec)
{
    ACE_OS::sleep(makeTimeValue(msec));
}

} // namespace nsrpc
