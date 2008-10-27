#ifndef SRPC_NAMINGSERVICE_H
#define SRPC_NAMINGSERVICE_H

#include <srpc/RpcInterface.h>
#include <srpc/RpcTypes.h>

namespace ns {

/// Naming Service RPC Interface
class NamingService
{
public:
    virtual ~NamingService() {}

    // = 요청

    /**
     * 서버에 새로운 name/value 항목을 설정합니다.
     * - 응답: onBind
     */
    DECLARE_SRPC_PURE_METHOD_2(NamingService, bind,
        srpc::RString, name, srpc::RString, value);

    /**
    * 기존 항목을 갱신하거나 새로운 name/value 항목을 설정합니다.
    * - 응답: onRebind
    */
    DECLARE_SRPC_PURE_METHOD_2(NamingService, rebind,
        srpc::RString, name, srpc::RString, value);

    /**
    * 항목을 삭제합니다
    * - 응답: onUnbind
    */
    DECLARE_SRPC_PURE_METHOD_1(NamingService, unbind, srpc::RString, name);

    /**
    * 데이터베이스 조회를 합니다.
    * - 응답: onResolve
    */
    DECLARE_SRPC_PURE_METHOD_1(NamingService, resolve, srpc::RString, name);

    // = 응답

    /**
     * bind 요청에 대한 응답을 받습니다.
     * @param succeeded 성공할 경우 true, 실패할 경우 false.
     */
    DECLARE_SRPC_PURE_METHOD_3(NamingService, onBind,
        srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded);

    /**
     * rebind 요청에 대한 응답을 받습니다.
     * @param succeeded 성공할 경우 true, 실패할 경우 false.
     */
    DECLARE_SRPC_PURE_METHOD_3(NamingService, onRebind,
        srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded);

    /**
     * unbind 요청에 대한 응답을 받습니다.
     * @param succeeded 성공할 경우 true, 실패할 경우 false.
     */
    DECLARE_SRPC_PURE_METHOD_2(NamingService, onUnbind,
        srpc::RString, name, srpc::RBool, succeeded);

    /**
    * resolve 요청에 대한 응답을 받습니다.
    * @param succeeded 성공할 경우 true, 실패할 경우 false.
    */
    DECLARE_SRPC_PURE_METHOD_3(NamingService, onResolve,
        srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded);
};

} /// namespace ns

#endif // SRPC_NAMINGSERVICE_H
