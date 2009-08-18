#ifndef NSRPC_SECURITYSERVICE_H
#define NSRPC_SECURITYSERVICE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <srpc/RpcInterface.h>
#include <srpc/RpcTypes.h>

namespace nsrpc
{

class Session;

/** @addtogroup protocol
* @{
*/

/**
 * @class RpcSecurityService
 *
 * ���� ���� RPC �������̽�
 * @internal
 */
class RpcSecurityService
{
public:
    virtual ~RpcSecurityService() {}

    // = request

    /**
     * ��Ŷ ��ȣȭ seed�� ������ ��û�Ѵ�(���� -> Ŭ���̾�Ʈ)
     * @param publicKey ������ ����Ű
     * @param seedForEncrypt ������ ��ȣȭ �õ�
     * @param seedForDecrypt ������ ��ȣȭ �õ�
     * - Response: onConfirmSeed
     */
    DECLARE_SRPC_METHOD_3(RpcSecurityService, exchangeSeed,
        srpc::RShortString, publicKey,
        srpc::RShortString, seedForEncrypt,
        srpc::RShortString, seedForDecrypt);

    /**
     * ����Ű ��ȯ�� ��û�Ѵ�(Ŭ���̾�Ʈ -> ����)
     * @param publicKey Ŭ���̾�Ʈ�� ����Ű
     * - Response: onConfirmPublicKey
     */
    DECLARE_SRPC_METHOD_1(RpcSecurityService, exchangePublicKey,
        srpc::RShortString, publicKey);

    // = response

    /**
     * seed ���濡 ���� ������ �Ѵ�(Ŭ���̾�Ʈ -> ����)
     */
    DECLARE_SRPC_METHOD_0(RpcSecurityService, onConfirmSeed);
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // NSRPC_SECURITYSERVICE_H
