#include "stdafx.h"
#include <srpc/detail/RpcCommand.h>
#include <srpc/OStream.h>
#include <srpc/detail/ForwardingFunctors.h>

namespace srpc {

void RpcCommand::marshal(OStream& ostream)
{
    ostream.write(rpcId_);
    marshaler_(ostream);
    ostream.align();
}

} // namespace srpc
