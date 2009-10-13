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
 * RPC를 처리하기 위한 AO Servant
 */
class RpcServant : public boost::noncopyable
{
public:
    virtual ~RpcServant() {}

    /// RPC 호출을 처리한다
    virtual void dispatch(ACE_Message_Block& mblock) = 0;
};

} // namespace sao

} // namespace nsrpc

#endif // NSRPC_SAO_RPCSERVANT_H
