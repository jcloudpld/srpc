#include "stdafx.h"
#include <nsrpc/sao/detail/PendingSessionRpcNetwork.h>
#include <nsrpc/sao/detail/MethodMemoryManager.h>
#include <nsrpc/sao/detail/RpcMethod.h>
#include <nsrpc/sao/Proxy.h>
#include <nsrpc/utility/Logger.h>
#include <nsrpc/utility/AceUtil.h>

namespace nsrpc
{

namespace sao
{

namespace
{

MethodMemoryManager<RpcMethod, Thread_Mutex_With_SpinLock> rpcMethodManager_;

} // namespace

PendingSessionRpcNetwork::PendingSessionRpcNetwork(Proxy& serviceProxy,
    bool useBitPacking) :
    SessionRpcNetwork(useBitPacking),
    serviceProxy_(serviceProxy)
{
}


PendingSessionRpcNetwork::~PendingSessionRpcNetwork()
{
}

// = nsrpc::SessionRpcNetwork overriding

bool PendingSessionRpcNetwork::messageArrived(nsrpc::CsMessageType messageType)
{
    if (nsrpc::mtSystem == messageType) {
        return handleMessageNow(*getRecvBlock());
    }

    RpcMethod* method = rpcMethodManager_.acquire();
    method->setParameter(this, getRecvBlock());
    serviceProxy_.schedule(method);
    return true;
}

// = RpcServant overriding

void PendingSessionRpcNetwork::dispatch(ACE_Message_Block& mblock)
{
    assert(mblock.data_block() != 0);

    if (! handleMessageNow(mblock)) {
        unmarshalingErrorOccurred();
    }
}

} // namespace sao

} // namespace nsrpc

