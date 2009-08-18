#ifndef NSRPC_SAO_RPCMETHOD_H
#define NSRPC_SAO_RPCMETHOD_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/sao/detail/MethodRequest.h>
#include <nsrpc/nsrpc.h>

class ACE_Message_Block;

namespace nsrpc
{

namespace sao
{

class RpcServant;

/**
 * @class RpcMethod
 *
 * RPC를 처리한다
 */
class NSRPC_API RpcMethod : public MethodRequest
{
public:
    RpcMethod(MethodReleaser* releaser) :
        MethodRequest(releaser),
        servant_(0),
        messageBlock_(0) {}

    virtual ~RpcMethod();

    /// RPC method의 파라미터를 설정한다
    void setParameter(RpcServant* servant, ACE_Message_Block* mblock);
private:
    virtual void call();
private:
    RpcServant* servant_;
    ACE_Message_Block* messageBlock_;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_RPCMETHOD_H
