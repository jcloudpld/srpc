#ifndef NSRPC_SAO_RPCSERVANT_H
#define NSRPC_SAO_RPCSERVANT_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <boost/noncopyable.hpp>

namespace nsrpc
{

namespace sao
{

/**
 * @class RpcServant
 * RPC�� ó���ϱ� ���� AO Servant
 */
class RpcServant : public boost::noncopyable
{
public:
    virtual ~RpcServant() {}

    /// RPC ȣ���� ó���Ѵ�
    virtual void dispatch(ACE_Message_Block& mblock) = 0;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_RPCSERVANT_H
