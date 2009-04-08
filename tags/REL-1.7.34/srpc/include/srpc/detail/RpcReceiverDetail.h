#ifndef SRPC_RPCRECEIVERDETAIL_H
#define SRPC_RPCRECEIVERDETAIL_H

#include "ReceivingFunctors.h"
#include "RpcEvent.h"
#include "RpcId.h"
#include "../utility/StlUtils.h"
#include "../ContainerTypes.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <map>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <cassert>

namespace srpc {

/** @addtogroup RpcReceiving
* RPC 수신 & 처리
* @{
*/

/**
 * @class RpcEventMap
 * RpcEvent 컨테이너
 */
class RpcEventMap
{
public:
    typedef srpc::Map<RpcId, RpcEvent*> RpcEvents;
    typedef RpcEvents::value_type value_type;
public:
    RpcEventMap(bool deleteRpcEvents) :
        deleteRpcEvents_(deleteRpcEvents) {}

    ~RpcEventMap() {
        if (deleteRpcEvents_) {
            std::for_each(rpcEvents_.begin(), rpcEvents_.end(),
                srpc::delete_second<RpcEvents::value_type>);
        }
    }

    void insert(RpcId rpcId, RpcEvent* event) {
        assert(event != 0);
        assert(! isExists(rpcId));
        rpcEvents_.insert(value_type(rpcId, event));
    }

    RpcEvent* getRpcEvent(RpcId rpcId) const {
        return search_map(rpcEvents_, rpcId, 0);
    }

    const RpcEvents& getRpcEvents() const {
        return rpcEvents_;
    }

    bool isEmpty() const {
        return rpcEvents_.empty();
    }
private:
    bool isExists(RpcId rpcId) const {
        return rpcEvents_.find(rpcId) != rpcEvents_.end();
    }
private:
    bool deleteRpcEvents_;
    RpcEvents rpcEvents_;
};

/**
 * @class EventRegister
 *
 * 생성자를 통해서 RpcEvent를 등록한다.
 */
template <class T, class RpcClass>
class EventRegister
{
public:
    EventRegister(RpcId rpcId, T* event) {
        RpcClass::getStaticEventMap().insert(rpcId, event);
    }
};

} // namespace srpc

// = Internal Macros

#define SRPC_RPC_EVENT(RpcClass, method) \
    RpcEvent_##RpcClass##_##method

#define SRPC_RPC_EVENT_INSTANCE(RpcClass, method) \
    RpcEvent_##RpcClass##_##method##_instance

#ifdef USE_BOOST_FAST_POOL_ALLOCATOR_FOR_SRPC
// boost::pool을 쓸 경우 singleton pool의 소멸자가 먼저 호출되어
// 메모리를 2번 해제하는 문제가 발생하여 메모리를 해제하지 않게함
// TODO: 메모리 릭 제거
#   define DEFINE_GET_SRPC_EVENT_MAP(RpcClass) \
        srpc::RpcEventMap& RpcClass :: getStaticEventMap() { \
            static srpc::RpcEventMap* RpcClass##_staticEventMap = \
                new srpc::RpcEventMap(false); \
            return *RpcClass##_staticEventMap; \
        }
#else
#   define DEFINE_GET_SRPC_EVENT_MAP(RpcClass) \
        srpc::RpcEventMap& RpcClass :: getStaticEventMap() { \
            static srpc::RpcEventMap RpcClass##_staticEventMap(false); \
            return RpcClass##_staticEventMap; \
        }
#endif

/// SRPC 헬퍼를 선언한다.
#define DECLARE_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass) \
    public: \
        static srpc::RpcEventMap& getStaticEventMap(); \
    protected: \
        virtual const srpc::RpcEventMap& getDefaultEventMap() const { \
            return RpcClass :: getStaticEventMap(); \
        } \
    private: \
        virtual void dispatch(srpc::RpcEvent& event, srpc::IStream& istream, \
            const void* rpcHint);

/// SRPC 헬퍼를 구현한다.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass) \
    DEFINE_GET_SRPC_EVENT_MAP(RpcClass); \
    void RpcClass::dispatch(srpc::RpcEvent& event, srpc::IStream& istream, \
        const void* rpcHint) { \
        event.dispatch(this, istream, rpcHint); \
    }

/// RPC 메쏘드를 등록한다
#define REGISTER_SRPC_METHOD(RpcClass, method, TypeList, suffix) \
    namespace { \
        class SRPC_RPC_EVENT(RpcClass, method) : public srpc::RpcEvent \
        { \
        public: \
            SRPC_RPC_EVENT(RpcClass, method)() : \
                srpc::RpcEvent(RpcClass::SRPC_GET_RPCID(method)()), \
                unmarshalFunctor_(&RpcClass::method##suffix) {} \
        private: \
            virtual srpc::RpcEvent* clone() const { \
                return new SRPC_RPC_EVENT(RpcClass, method); \
            } \
            virtual srpc::ReceivingFunctor& getDispatcher() { \
                return unmarshalFunctor_; \
            } \
        private: \
            srpc::ReceivingFunctorT<RpcClass, TypeList > \
                unmarshalFunctor_; \
        }; \
        static SRPC_RPC_EVENT(RpcClass, method) \
            SRPC_RPC_EVENT_INSTANCE(RpcClass, method); \
        srpc::EventRegister<SRPC_RPC_EVENT(RpcClass, method), RpcClass> \
            RpcClass##_##method##_EventRegister( \
                RpcClass::SRPC_GET_RPCID(method)(), \
                &SRPC_RPC_EVENT_INSTANCE(RpcClass, method)); \
    }

// = IMPLEMENT_SRPC_METHOD_DETAIL_n

/// 파라미터가 없는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_0(), ); \
    void RpcClass::method(const void* rpcHint)

/// 파라미터가 1개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        P1, p1, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_1(P1), ) \
    void RpcClass::method(const P1& p1, const void* rpcHint)

/// 파라미터가 2개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        P1, p1, P2, p2, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_2(P1, P2), ) \
    void RpcClass::method(const P1& p1, const P2& p2, const void* rpcHint)

/// 파라미터가 3개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_3(P1, P2, P3), ) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const void* rpcHint)

/// 파라미터가 4개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_4(P1, P2, P3, P4), ) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const void* rpcHint)

/// 파라미터가 5개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_5(P1, P2, P3, P4, P5), ) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const void* rpcHint)

/// 파라미터가 6개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6), ) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const void* rpcHint)

/// 파라미터가 7개 있는 RPC Method를 구현한다
#define IMPLEMENT_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7), ) \
    void RpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const P7& p7, \
        const void* rpcHint)

/** @} */ // addtogroup RpcReceiving

#endif // SRPC_RPCRECEIVERDETAIL_H
