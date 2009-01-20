#include "stdafx.h"
#include <srpc/detail/RpcCommand.h>
#include <srpc/OStream.h>
#include <srpc/detail/ForwardingFunctors.h>

namespace srpc {

void RpcCommand::marshal(OStream& ostream)
{
    ForwardingFunctor& marshaler = getMarshaler();

    ostream.write(rpcId_);
    marshaler.marshal(ostream);
    ostream.align();
}

} // namespace srpc
