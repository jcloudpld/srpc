#include "stdafx.h"
#include "Scheduler.h"
#include <nsrpc/sao/Proxy.h>

namespace nsrpc
{

namespace sao
{

void Proxy::schedule(MethodRequest* method)
{
    assert(method != 0);
    scheduler_.schedule(method);
}

} // namespace sao

} // namespace nsrpc
