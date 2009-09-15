#ifndef NSRPC_SYSTEMUTIL_H
#define NSRPC_SYSTEMUTIL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/// msec��ŭ ���� �����带 ������Ų��.
void NSRPC_API pause(size_t msec);

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_SYSTEMUTIL_H
