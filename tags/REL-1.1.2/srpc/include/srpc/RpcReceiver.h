#ifndef SRPC_RPCRECEIVER_H
#define SRPC_RPCRECEIVER_H

#include "detail/RpcReceiverDetail.h"
#include <cassert>

namespace srpc {

class RpcNetwork;
class IStream;

/** @addtogroup RpcReceiving
* RPC ���� & ó��
* @{
*/

/**
 * @class RpcReceiver
 *
 * rpc receiver
 * - ������ ������ ������ �޾� ó���Ѵ�
 */
class SRPC_API RpcReceiver : public boost::noncopyable
{
public:
    RpcReceiver(RpcNetwork* rpcNetwork = 0);
    virtual ~RpcReceiver();

    /// RPC ��û�� �Ľ��Ͽ� RPC method�� �����Ѵ�.
    bool handle(const RpcId& rpcId, IStream& istream, const void* rpcHint = 0);

    void setRpcNetwork(RpcNetwork* rpcNetwork);
    void resetRpcNetwork();
protected:
    virtual const RpcEventMap& getDefaultEventMap() const = 0;
private:
    virtual void dispatch(RpcEvent& rpcEvent, IStream& istream,
        const void* rpcHint) = 0;
protected:
    void setRpcEvent(const srpc::RpcId& rpcId, srpc::RpcEvent* event) {
        eventMap_.insert(rpcId, event);
    }
private:
    RpcEvent* lookupEvent(const RpcId& rpcId);
    bool handleEvent(RpcEvent& rpcEvent, IStream& istream,
        const void* rpcHint);
private:
    RpcNetwork* rpcNetwork_;
    RpcEventMap eventMap_;
};

} // namespace srpc

// = Macros

/// SRPC ���۸� �����Ѵ�.
#define DECLARE_SRPC_EVENT_DISPATCHER(rpcClass) \
    DECLARE_SRPC_EVENT_DISPATCHER_DETAIL(rpcClass)

/// SRPC ���۸� �����Ѵ�.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER(rpcClass) \
    IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(rpcClass)

// = IMPLEMENT_SRPC_METHOD_n

/// ��Ʈ�� ���� �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_0(rpcClass, method) \
    IMPLEMENT_SRPC_METHOD_DETAIL_0(rpcClass, method, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 1�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_1(rpcClass, method, T1, p1) \
    IMPLEMENT_SRPC_METHOD_DETAIL_1(rpcClass, method, T1, p1, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 2�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_2(rpcClass, method, T1, p1, T2, p2) \
    IMPLEMENT_SRPC_METHOD_DETAIL_2(rpcClass, method, T1, p1, T2, p2, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 3�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    IMPLEMENT_SRPC_METHOD_DETAIL_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 4�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    IMPLEMENT_SRPC_METHOD_DETAIL_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 5�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    IMPLEMENT_SRPC_METHOD_DETAIL_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 6�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    IMPLEMENT_SRPC_METHOD_DETAIL_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 7�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    IMPLEMENT_SRPC_METHOD_DETAIL_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, )

// = IMPLEMENT_SRPC_METHOD_WITH_HINT_n

/// ��Ʈ�� �ְ� �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_0(rpcClass, method) \
    IMPLEMENT_SRPC_METHOD_DETAIL_0(rpcClass, method, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 1�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_1(rpcClass, method, T1, p1) \
    IMPLEMENT_SRPC_METHOD_DETAIL_1(rpcClass, method, \
        T1, p1, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 2�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_2(rpcClass, method, \
        T1, p1, T2, p2) \
    IMPLEMENT_SRPC_METHOD_DETAIL_2(rpcClass, method, \
        T1, p1, T2, p2, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 3�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    IMPLEMENT_SRPC_METHOD_DETAIL_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 4�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    IMPLEMENT_SRPC_METHOD_DETAIL_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 5�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    IMPLEMENT_SRPC_METHOD_DETAIL_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 6�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    IMPLEMENT_SRPC_METHOD_DETAIL_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 7�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    IMPLEMENT_SRPC_METHOD_DETAIL_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, rpcHint)

/** @} */ // addtogroup RpcReceiving

#endif // SRPC_RPCRECEIVER_H
