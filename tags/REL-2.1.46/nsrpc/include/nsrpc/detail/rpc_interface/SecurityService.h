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
 * 보안 관련 RPC 인터페이스
 * @internal
 */
class RpcSecurityService
{
public:
    virtual ~RpcSecurityService() {}

    // = request

    /**
     * 패킷 암호화 seed값 변경을 요청한다(서버 -> 클라이언트)
     * @param publicKey 서버의 공개키
     * @param seedForEncrypt 서버의 암호화 시드
     * @param seedForDecrypt 서버의 복호화 시드
     * - Response: onConfirmSeed
     */
    DECLARE_SRPC_METHOD_3(RpcSecurityService, exchangeSeed,
        srpc::RShortString, publicKey,
        srpc::RShortString, seedForEncrypt,
        srpc::RShortString, seedForDecrypt);

    /**
     * 공개키 교환을 요청한다(클라이언트 -> 서버)
     * @param publicKey 클라이언트의 공개키
     * - Response: onConfirmPublicKey
     */
    DECLARE_SRPC_METHOD_1(RpcSecurityService, exchangePublicKey,
        srpc::RShortString, publicKey);

    // = response

    /**
     * seed 변경에 대한 응답을 한다(클라이언트 -> 서버)
     */
    DECLARE_SRPC_METHOD_0(RpcSecurityService, onConfirmSeed);
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // NSRPC_SECURITYSERVICE_H
