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
 * RPC ��û�� �ٷ� ó������ �ʰ� Active Object�� ���� ó���Ѵ�
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
     * �ý��� �޼���(mtSystem)�� �ٷ� ó���ϰ�, �Ϲ� �޼���(mtGeneral)�� AO��
     * ���� ��ȸ ó���Ѵ�.
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
