#ifndef NSRPC_SECURITYSERVICE_H
#define NSRPC_SECURITYSERVICE_H

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
 * ���� ���� RPC �������̽� (���� ����)
 * @internal
 */
class RpcSecurityService
{
public:
    virtual ~RpcSecurityService() {}

    // = request

    /**
     * ��Ŷ ��ȣȭ seed�� ������ ��û�Ѵ�(���� -> Ŭ���̾�Ʈ)
     * @param encryptSeed ������ ��ȣȭ �õ�
     * @param decryptSeed ������ ��ȣȭ �õ�
     * - Response: onConfirmSeed
     */
    DECLARE_SRPC_PURE_METHOD_2(RpcSecurityService, exchangeSeed,
        srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);

    // = response

    /**
     * seed ���濡 ���� ������ �Ѵ�(Ŭ���̾�Ʈ -> ����)
     * @param encryptSeed Ŭ���̾�Ʈ�� ��ȣȭ �õ�
     */
    DECLARE_SRPC_PURE_METHOD_1(RpcSecurityService, onConfirmSeed,
        srpc::RShortString, encryptSeed);
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // NSRPC_SECURITYSERVICE_H
