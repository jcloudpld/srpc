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
* RPC ���� & ó��
* @{
*/

/**
 * @class RpcEventMap
 * RpcEvent �����̳�
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

    void insert(RpcId rpcId, RpcEvent& event) {
        assert(! isExists(rpcId));
        rpcEvents_.insert(value_type(rpcId, &event));
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
 * �����ڸ� ���ؼ� RpcEvent�� ����Ѵ�.
 */
template <class T, class RpcClass>
class EventRegister
{
public:
    EventRegister(RpcId rpcId, T& event) {
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
// boost::pool�� �� ��� singleton pool�� �Ҹ��ڰ� ���� ȣ��Ǿ�
// �޸𸮸� 2�� �����ϴ� ������ �߻��Ͽ� �޸𸮸� �������� �ʰ���
// TODO: �޸� �� ����
#   define DEFINE_GET_SRPC_EVENT_MAP(RpcClass) \
        srpc::RpcEventMap& RpcClass :: getStaticEventMap() { \
            static srpc::RpcEventMap* RpcClass##_staticEventMap = \
                new srpc::RpcEventMap(false); \
            return *RpcClass##_staticEventMap; \
        }
#else
#   define DEFINE_GET_SRPC_EVENT_MAP(RpcClass) \
        static srpc::RpcEventMap RpcClass##_staticEventMap(false); \
        srpc::RpcEventMap& RpcClass :: getStaticEventMap() { \
            return RpcClass##_staticEventMap; \
        }
#endif

/// SRPC ���۸� �����Ѵ�.
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

/// SRPC ���۸� �����Ѵ�.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(RpcClass) \
    DEFINE_GET_SRPC_EVENT_MAP(RpcClass); \
    void RpcClass::dispatch(srpc::RpcEvent& event, srpc::IStream& istream, \
        const void* rpcHint) { \
        event.dispatch(this, istream, rpcHint); \
    }

/// RPC �޽�带 ����Ѵ�
#define REGISTER_SRPC_METHOD(RpcClass, method, TypeList, suffix) \
    namespace { \
        class SRPC_RPC_EVENT(RpcClass, method) : public srpc::RpcEvent \
        { \
        public: \
            SRPC_RPC_EVENT(RpcClass, method)() : \
                srpc::RpcEvent(RpcClass::SRPC_GET_RPCID(method)()), \
                unmarshalFunctor_(&RpcClass::method##suffix) {} \
        private: \
            virtual std::auto_ptr<RpcEvent> clone() const { \
                return std::auto_ptr<RpcEvent>( \
                    new SRPC_RPC_EVENT(RpcClass, method)); \
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
                SRPC_RPC_EVENT_INSTANCE(RpcClass, method)); \
    }

// = RECEIVE_SRPC_METHOD_DETAIL_n

/// �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_0(RpcClass, method, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_0(), ); \
    void RpcClass::method(const void* rpcHint)

/// �Ķ���Ͱ� 1�� �ִ� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_1(RpcClass, method, \
        P1, p1, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_1(P1), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, const void* rpcHint)

/// �Ķ���Ͱ� 2�� �ִ� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_2(RpcClass, method, \
        P1, p1, P2, p2, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_2(P1, P2), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        const void* rpcHint)

/// �Ķ���Ͱ� 3�� �ִ� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_3(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_3(P1, P2, P3), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, const void* rpcHint)

/// �Ķ���Ͱ� 4�� �ִ� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_4(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, SRPC_TYPELIST_4(P1, P2, P3, P4), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, const void* rpcHint)

/// �Ķ���Ͱ� 5�� �ִ� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_5(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_5(P1, P2, P3, P4, P5), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        const void* rpcHint)

/// �Ķ���Ͱ� 6�� �ִ� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_6(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, const void* rpcHint)

/// �Ķ���Ͱ� 7�� �ִ� RPC Method�� �����Ѵ�
#define RECEIVE_SRPC_METHOD_DETAIL_7(RpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, rpcHint) \
    REGISTER_SRPC_METHOD(RpcClass, method, \
        SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7), ) \
    void RpcClass::method(SRPC_PARAM_TYPE(P1) p1, SRPC_PARAM_TYPE(P2) p2, \
        SRPC_PARAM_TYPE(P3) p3, SRPC_PARAM_TYPE(P4) p4, SRPC_PARAM_TYPE(P5) p5, \
        SRPC_PARAM_TYPE(P6) p6, SRPC_PARAM_TYPE(P7) p7, const void* rpcHint)

/** @} */ // addtogroup RpcReceiving

#endif // SRPC_RPCRECEIVERDETAIL_H
