#ifndef NSRPC_RPC_PENDINGSESSIONRPCNETWORK_H
#define NSRPC_RPC_PENDINGSESSIONRPCNETWORK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "RpcServant.h"
#include <nsrpc/nsrpc.h>
#include <nsrpc/detail/SessionRpcNetwork.h>

class ACE_Null_Mutex;

namespace nsrpc
{

namespace sao
{

class Proxy;
class RpcMethod;

/**
 * @class PendingSessionRpcNetwork
 * RPC 요청을 바로 처리하지 않고 Active Object를 통해 처리한다
 */
class NSRPC_API PendingSessionRpcNetwork :
    public nsrpc::SessionRpcNetwork,
    private RpcServant
{
public:
    PendingSessionRpcNetwork(Proxy& serviceProxy, bool useBitPacking);

    virtual ~PendingSessionRpcNetwork();
private:
    // = nsrpc::SessionRpcNetwork overriding

    /**
     * 시스템 메세지(mtSystem)는 바로 처리하고, 일반 메세지(mtGeneral)은 AO를
     * 통해 우회 처리한다.
     */
    virtual bool messageArrived(nsrpc::CsMessageType messageType);

    // = RpcServant overriding
    virtual void dispatch(ACE_Message_Block& mblock);
private:
    Proxy& serviceProxy_;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_RPC_PENDINGSESSIONRPCNETWORK_H
