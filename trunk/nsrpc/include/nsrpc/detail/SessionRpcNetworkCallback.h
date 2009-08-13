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
     * �޼����� �����Ѵ�.
     * - ȣ���ڴ� mblock�� �޸� �������� �Ѱ���� �Ѵ�.
     */
    virtual void sendNow(ACE_Message_Block& mblock,
        CsMessageType messageType) = 0;

    /// ��Ŷ ���ڵ� �� ������ �߻��Ͽ���.
    virtual void marshalingErrorOccurred() = 0;

    /// ��Ŷ ���ڵ� �� ������ �߻��Ͽ���.
    virtual void unmarshalingErrorOccurred() = 0;
};

/** @} */ // addtogroup session

} // namespace nsrpc

#endif // NSRPC_SESSIONRPCNETWORKCALLBACK_H
