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

    // = ��û

    /**
     * ������ ���ο� name/value �׸��� �����մϴ�.
     * - ����: onBind
     */
    DECLARE_SRPC_PURE_METHOD_2(NamingService, bind,
        srpc::RString, name, srpc::RString, value);

    /**
    * ���� �׸��� �����ϰų� ���ο� name/value �׸��� �����մϴ�.
    * - ����: onRebind
    */
    DECLARE_SRPC_PURE_METHOD_2(NamingService, rebind,
        srpc::RString, name, srpc::RString, value);

    /**
    * �׸��� �����մϴ�
    * - ����: onUnbind
    */
    DECLARE_SRPC_PURE_METHOD_1(NamingService, unbind, srpc::RString, name);

    /**
    * �����ͺ��̽� ��ȸ�� �մϴ�.
    * - ����: onResolve
    */
    DECLARE_SRPC_PURE_METHOD_1(NamingService, resolve, srpc::RString, name);

    // = ����

    /**
     * bind ��û�� ���� ������ �޽��ϴ�.
     * @param succeeded ������ ��� true, ������ ��� false.
     */
    DECLARE_SRPC_PURE_METHOD_3(NamingService, onBind,
        srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded);

    /**
     * rebind ��û�� ���� ������ �޽��ϴ�.
     * @param succeeded ������ ��� true, ������ ��� false.
     */
    DECLARE_SRPC_PURE_METHOD_3(NamingService, onRebind,
        srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded);

    /**
     * unbind ��û�� ���� ������ �޽��ϴ�.
     * @param succeeded ������ ��� true, ������ ��� false.
     */
    DECLARE_SRPC_PURE_METHOD_2(NamingService, onUnbind,
        srpc::RString, name, srpc::RBool, succeeded);

    /**
    * resolve ��û�� ���� ������ �޽��ϴ�.
    * @param succeeded ������ ��� true, ������ ��� false.
    */
    DECLARE_SRPC_PURE_METHOD_3(NamingService, onResolve,
        srpc::RString, name, srpc::RString, value, srpc::RBool, succeeded);
};

} /// namespace ns

#endif // SRPC_NAMINGSERVICE_H
