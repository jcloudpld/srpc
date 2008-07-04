#ifndef SRPC_RPCFORWARDER_H
#define SRPC_RPCFORWARDER_H

#include "config/config.h"
#include "detail/RpcForwarderDetail.h"
#include <boost/noncopyable.hpp>
#include <cassert>

namespace srpc {

class RpcNetwork;

/** @addtogroup RpcForwarding
* RPC �۽�
* @{
*/

/**
 * @class RpcForwarder
 *
 * rpc forwarder
 * - Ŭ���̾�Ʈ�� ��û�� ��Ʈ������ ��ȯ�� �� ������ �����Ѵ�
 */
class SRPC_API RpcForwarder : public boost::noncopyable
{
public:
    RpcForwarder(RpcNetwork* rpcNetwork = 0) :
        rpcNetwork_(rpcNetwork) {}

    /**
     * RPC �޼����� �����Ѵ�.
     * @param command RPC Command �ν��Ͻ�.
     * @param packetType ��Ŷ�� ��� ���� ���ΰ�? (TCP/IP���� �ǹ̰� ����)
     * @param rpcHint RpcNetwork���� �ѱ� ��Ʈ ������.
     */
    void forward(RpcCommand& command, RpcPacketType packetType,
        const void* rpcHint);

    /// �޼��� ������ ���� RpcNetwork �ν��Ͻ��� �����Ѵ�
    void setRpcNetwork(RpcNetwork* rpcNetwork) {
        assert(rpcNetwork != 0);
        rpcNetwork_ = rpcNetwork;
    }

    void resetRpcNetwork() {
        rpcNetwork_ = 0;
    }
private:
    virtual void onForwarding(const RRpcId& rpcId) { rpcId; }
private:
    RpcNetwork* rpcNetwork_;
};

} // namespace srpc

// = Rpc request Macros

// = DEFINE_SRPC_METHOD_n

/// �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�.
#define DEFINE_SRPC_METHOD_0(RpcClass, method) \
    DEFINE_SRPC_METHOD_DETAIL_0(RpcClass, method, srpc::ptReliable)

/// �Ķ���Ͱ� 1���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_METHOD_1(RpcClass, method, T1, p1) \
    DEFINE_SRPC_METHOD_DETAIL_1(RpcClass, method, T1, p1, srpc::ptReliable)

/// �Ķ���Ͱ� 2���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_METHOD_2(RpcClass, method, T1, p1, T2, p2) \
    DEFINE_SRPC_METHOD_DETAIL_2(RpcClass, method, T1, p1, T2, p2, \
        srpc::ptReliable)

/// �Ķ���Ͱ� 3���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_METHOD_3(RpcClass, method, T1, p1, T2, p2, T3, p3) \
    DEFINE_SRPC_METHOD_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        srpc::ptReliable)

/// �Ķ���Ͱ� 4���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_METHOD_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4) \
    DEFINE_SRPC_METHOD_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, srpc::ptReliable)

/// �Ķ���Ͱ� 5���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_METHOD_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5) \
    DEFINE_SRPC_METHOD_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, srpc::ptReliable)

/// �Ķ���Ͱ� 6���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_METHOD_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6) \
    DEFINE_SRPC_METHOD_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, srpc::ptReliable)

/// �Ķ���Ͱ� 7���� RPC Method�� �����Ѵ�
#define DEFINE_SRPC_METHOD_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7) \
    DEFINE_SRPC_METHOD_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
    T4, p4, T5, p5, T6, p6, T7, p7, srpc::ptReliable)

// = SRPC_METHOD_PROTOTYPE_n

/// ��Ʈ�� ���� �Ķ���Ͱ� ���� RPC Method Prototype�� �����Ѵ�.
#define SRPC_METHOD_PROTOTYPE_0(RpcClass, method) \
    SRPC_METHOD_PROTOTYPE_DETAIL_0(RpcClass, method, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 1���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_1(RpcClass, method, T1, p1) \
    SRPC_METHOD_PROTOTYPE_DETAIL_1(RpcClass, method, T1, p1, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 2���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_2(RpcClass, method, T1, p1, T2, p2) \
    SRPC_METHOD_PROTOTYPE_DETAIL_2(RpcClass, method, T1, p1, T2, p2, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 3���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_3(RpcClass, method, T1, p1, T2, p2, T3, p3) \
    SRPC_METHOD_PROTOTYPE_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 4���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4) \
    SRPC_METHOD_PROTOTYPE_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 5���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5) \
    SRPC_METHOD_PROTOTYPE_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 6���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6) \
    SRPC_METHOD_PROTOTYPE_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, )

/// ��Ʈ�� ���� �Ķ���Ͱ� 7���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7) \
    SRPC_METHOD_PROTOTYPE_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, )

// = SRPC_METHOD_PROTOTYPE_WITH_HINT_n

/// ��Ʈ�� �ְ� �Ķ���Ͱ� ���� RPC Method Prototype�� �����Ѵ�.
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_0(RpcClass, method) \
    SRPC_METHOD_PROTOTYPE_DETAIL_0(RpcClass, method, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 1���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_1(RpcClass, method, T1, p1) \
    SRPC_METHOD_PROTOTYPE_DETAIL_1(RpcClass, method, T1, p1, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 2���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_2(RpcClass, method, T1, p1, T2, p2) \
    SRPC_METHOD_PROTOTYPE_DETAIL_2(RpcClass, method, T1, p1, T2, p2, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 3���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_3(RpcClass, method, T1, p1, T2, p2, \
        T3, p3) \
    SRPC_METHOD_PROTOTYPE_DETAIL_3(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 4���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_4(RpcClass, method, T1, p1, T2, p2, \
        T3, p3, T4, p4) \
    SRPC_METHOD_PROTOTYPE_DETAIL_4(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 5���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_5(RpcClass, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5) \
    SRPC_METHOD_PROTOTYPE_DETAIL_5(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 6���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_6(RpcClass, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6) \
    SRPC_METHOD_PROTOTYPE_DETAIL_6(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, rpcHint)

/// ��Ʈ�� �ְ� �Ķ���Ͱ� 7���� RPC Method Prototype�� �����Ѵ�
#define SRPC_METHOD_PROTOTYPE_WITH_HINT_7(RpcClass, method, T1, p1, T2, p2, \
        T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    SRPC_METHOD_PROTOTYPE_DETAIL_7(RpcClass, method, T1, p1, T2, p2, T3, p3, \
        T4, p4, T5, p5, T6, p6, T7, p7, rpcHint)

/** @} */ // addtogroup RpcForwarding

#endif // SRPC_RPCFORWARDER_H
