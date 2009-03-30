#ifndef NSRPC_SYSTEMUTIL_H
#define NSRPC_SYSTEMUTIL_H

#include "../nsrpc.h"

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/// msec만큼 현재 쓰레드를 중지시킨다.
void NSRPC_API pause(size_t msec);

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_SYSTEMUTIL_H
