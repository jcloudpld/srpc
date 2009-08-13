#ifndef NSRPC_SESSIONRPCNETWORKCALLBACK_H
#define NSRPC_SESSIONRPCNETWORKCALLBACK_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "CsProtocol.h"
#include <boost/noncopyable.hpp>

class ACE_Message_Block;

namespace nsrpc
{

/** @addtogroup session
* @{
*/

/**
 * @class SessionRpcNetworkCallback
 *
 * RpcNetwork Helper class
 */
class SessionRpcNetworkCallback : public boost::noncopyable
{
public:
    virtual ~SessionRpcNetworkCallback() {}

    /**
     * 메세지를 전송한다.
     * - 호출자는 mblock의 메모리 소유권을 넘겨줘야 한다.
     */
    virtual void sendNow(ACE_Message_Block& mblock,
        CsMessageType messageType) = 0;

    /// 패킷 인코딩 중 에러가 발생하였다.
    virtual void marshalingErrorOccurred() = 0;

    /// 패킷 디코딩 중 에러가 발생하였다.
    virtual void unmarshalingErrorOccurred() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONRPCNETWORKCALLBACK_H
