#ifndef SRPC_RPCRECEIVER_H
#define SRPC_RPCRECEIVER_H

#include "detail/RpcReceiverDetail.h"
#include <cassert>

namespace srpc {

class RpcNetwork;
class IStream;

/** @addtogroup RpcReceiving
* RPC 수신 & 처리
* @{
*/

/**
 * @class RpcReceiver
 *
 * rpc receiver
 * - 서버로 부터의 응답을 받아 처리한다
 */
class SRPC_API RpcReceiver : public boost::noncopyable
{
public:
    RpcReceiver(RpcNetwork* rpcNetwork = 0);
    virtual ~RpcReceiver();

    /// RPC 요청을 파싱하여 RPC method를 실행한다.
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

/// SRPC 헬퍼를 선언한다.
#define DECLARE_SRPC_EVENT_DISPATCHER(rpcClass) \
    DECLARE_SRPC_EVENT_DISPATCHER_DETAIL(rpcClass)

/// SRPC 헬퍼를 구현한다.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER(rpcClass) \
    IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(rpcClass)

// = IMPLEMENT_SRPC_METHOD_n

/// 힌트가 없고 파라미터가 없는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_0(rpcClass, method) \
    IMPLEMENT_SRPC_METHOD_DETAIL_0(rpcClass, method, )

/// 힌트가 없고 파라미터가 1개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_1(rpcClass, method, T1, p1) \
    IMPLEMENT_SRPC_METHOD_DETAIL_1(rpcClass, method, T1, p1, )

/// 힌트가 없고 파라미터가 2개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_2(rpcClass, method, T1, p1, T2, p2) \
    IMPLEMENT_SRPC_METHOD_DETAIL_2(rpcClass, method, T1, p1, T2, p2, )

/// 힌트가 없고 파라미터가 3개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    IMPLEMENT_SRPC_METHOD_DETAIL_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, )

/// 힌트가 없고 파라미터가 4개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    IMPLEMENT_SRPC_METHOD_DETAIL_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, )

/// 힌트가 없고 파라미터가 5개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    IMPLEMENT_SRPC_METHOD_DETAIL_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, )

/// 힌트가 없고 파라미터가 6개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    IMPLEMENT_SRPC_METHOD_DETAIL_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, )

/// 힌트가 없고 파라미터가 7개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    IMPLEMENT_SRPC_METHOD_DETAIL_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, )

// = IMPLEMENT_SRPC_METHOD_WITH_HINT_n

/// 힌트가 있고 파라미터가 없는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_0(rpcClass, method) \
    IMPLEMENT_SRPC_METHOD_DETAIL_0(rpcClass, method, rpcHint)

/// 힌트가 있고 파라미터가 1개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_1(rpcClass, method, T1, p1) \
    IMPLEMENT_SRPC_METHOD_DETAIL_1(rpcClass, method, \
        T1, p1, rpcHint)

/// 힌트가 있고 파라미터가 2개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_2(rpcClass, method, \
        T1, p1, T2, p2) \
    IMPLEMENT_SRPC_METHOD_DETAIL_2(rpcClass, method, \
        T1, p1, T2, p2, rpcHint)

/// 힌트가 있고 파라미터가 3개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3) \
    IMPLEMENT_SRPC_METHOD_DETAIL_3(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, rpcHint)

/// 힌트가 있고 파라미터가 4개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4) \
    IMPLEMENT_SRPC_METHOD_DETAIL_4(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, rpcHint)

/// 힌트가 있고 파라미터가 5개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5) \
    IMPLEMENT_SRPC_METHOD_DETAIL_5(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, rpcHint)

/// 힌트가 있고 파라미터가 6개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6) \
    IMPLEMENT_SRPC_METHOD_DETAIL_6(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, rpcHint)

/// 힌트가 있고 파라미터가 7개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_WITH_HINT_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7) \
    IMPLEMENT_SRPC_METHOD_DETAIL_7(rpcClass, method, \
        T1, p1, T2, p2, T3, p3, T4, p4, T5, p5, T6, p6, T7, p7, rpcHint)

/** @} */ // addtogroup RpcReceiving

#endif // SRPC_RPCRECEIVER_H
