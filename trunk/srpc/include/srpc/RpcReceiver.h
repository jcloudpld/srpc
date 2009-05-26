#ifndef SRPC_RPCRECEIVER_H
#define SRPC_RPCRECEIVER_H

#include "srpc.h"
#include "srpc_macros.h"
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
    bool handle(RpcId rpcId, IStream& istream, const void* rpcHint = 0);

    void setRpcNetwork(RpcNetwork* rpcNetwork);
    void resetRpcNetwork();

    RpcIds getRegisteredRpcIds() const;
protected:
    virtual const RpcEventMap& getDefaultEventMap() const = 0;
private:
    virtual void onReceiving(const RRpcId& rpcId) { SRPC_UNUSED_ARG(rpcId); }

    virtual void dispatch(RpcEvent& rpcEvent, IStream& istream,
        const void* rpcHint) = 0;
private:
    void cloneRpcEvents();
    void insertRpcEvent(RpcId rpcId, RpcEvent* event) {
        eventMap_.insert(rpcId, event);
    }
    RpcEvent* lookupEvent(RpcId rpcId) const {
        return eventMap_.getRpcEvent(rpcId);
    }
    bool handleEvent(RpcEvent& rpcEvent, IStream& istream,
        const void* rpcHint);
    void resetRpcEvents() {
        eventMap_.reset();
    }
    bool shouldCloneRpcEvents() const {
        return eventMap_.isEmpty();
    }
private:
    RpcNetwork* rpcNetwork_;
    RpcEventMap eventMap_;
};

} // namespace srpc

// = Macros

/// SRPC ���۸� �����Ѵ�.
#define DECLARE_SRPC_EVENT_DISPATCHER(RpcClass) \
    DECLARE_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass)

/// SRPC ���۸� �����Ѵ�.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER(RpcClass) \
    IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass)

// = IMPLEMENT_SRPC_METHOD_n

/// ��Ʈ�� ���� �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_0(RpcClass, method) \
    IMPLEMENT_SRPC_METHOD_DETAIL_0(RpcClass, method, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 1�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_1(RpcClass, method, T1, p1) \
    IMPLEMENT_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 2�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_2(RpcClass, method, T1, p1, T2, p2) \
    IMPLEMENT_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 3�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    IMPLEMENT_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 4�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    IMPLEMENT_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 5�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    IMPLEMENT_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 6�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    IMPLEMENT_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 7�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    IMPLEMENT_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, )

// = IMPLEMENT_SRPC_METHOD_WITH_HINT_n

/// ��Ʈ�� �ְ� �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_0(RpcClass, method) \
    IMPLEMENT_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 1�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_1(RpcClass, method, T1, p1) \
    IMPLEMENT_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        T1, p1, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 2�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_2(RpcClass, method, \
        T1, p1, T2, p2) \
    IMPLEMENT_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        T1, p1, T2, p2, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 3�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    IMPLEMENT_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 4�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    IMPLEMENT_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 5�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    IMPLEMENT_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 6�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    IMPLEMENT_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 7�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    IMPLEMENT_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, rpcHint)

/** @} */ // addtogroup RpcReceiving

#endif // SRPC_RPCRECEIVER_H
