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
 * 보안 관련 RPC 인터페이스 (서버 기준)
 * @internal
 */
class RpcSecurityService
{
public:
    virtual ~RpcSecurityService() {}

    // = request

    /**
     * 패킷 암호화 seed값 변경을 요청한다(서버 -> 클라이언트)
     * @param encryptSeed 서버의 암호화 시드
     * @param decryptSeed 서버의 복호화 시드
     * - Response: onConfirmSeed
     */
    DECLARE_SRPC_PURE_METHOD_2(RpcSecurityService, exchangeSeed,
        srpc::RShortString, encryptSeed, srpc::RShortString, decryptSeed);

    // = response

    /**
     * seed 변경에 대한 응답을 한다(클라이언트 -> 서버)
     * @param encryptSeed 클라이언트의 암호화 시드
     */
    DECLARE_SRPC_PURE_METHOD_1(RpcSecurityService, onConfirmSeed,
        srpc::RShortString, encryptSeed);
};

/** @} */ // addtogroup protocol

} // namespace nsrpc

#endif // NSRPC_SECURITYSERVICE_H
