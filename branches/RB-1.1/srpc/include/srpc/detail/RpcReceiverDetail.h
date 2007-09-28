#ifndef SRPC_RPCRECEIVERDETAIL_H
#define SRPC_RPCRECEIVERDETAIL_H

#include "ReceivingFunctors.h"
#include "RpcEvent.h"
#include "RpcId.h"
#include "utility/StlUtils.h"
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
    /// ����! boost::pool�� srpc::Map�� ���� �� ��� ������ �߻��� �� �ִ�.
    typedef std::map<RpcId, RpcEvent*> RpcEvents;
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

    void insert(const RpcId& rpcId, RpcEvent* event) {
        assert(event != 0);
        assert(! isExists(rpcId));
        rpcEvents_.insert(value_type(rpcId, event));
    }

    RpcEvent* getRpcEvent(const RpcId& rpcId) const {
        return search_map(rpcEvents_, rpcId, 0);
    }
private:
    bool isExists(const RpcId& rpcId) const {
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
    EventRegister(const RpcId& rpcId, T* event) {
        RpcClass::getStaticEventMap().insert(rpcId, event);
    }
};

} // namespace srpc

// = Internal Macros

#define SRPC_RPC_EVENT(rpcClass, method) \
    RpcEvent_##rpcClass##_##method

/// SRPC ���۸� �����Ѵ�.
#define DECLARE_SRPC_EVENT_DISPATCHER_DETAIL(rpcClass) \
    public: \
        static srpc::RpcEventMap& getStaticEventMap(); \
    protected: \
        virtual const srpc::RpcEventMap& getDefaultEventMap() const { \
            return rpcClass :: getStaticEventMap(); \
        } \
    private: \
        virtual void dispatch(srpc::RpcEvent& event, srpc::IStream& istream, \
            const void* rpcHint);

/// SRPC ���۸� �����Ѵ�.
#define IMPLEMENT_SRPC_EVENT_DISPATCHER_DETAIL(rpcClass) \
    srpc::RpcEventMap& rpcClass :: getStaticEventMap() { \
        static srpc::RpcEventMap staticEventMap(false); \
        return staticEventMap; \
    } \
    void rpcClass::dispatch(srpc::RpcEvent& event, srpc::IStream& istream, \
        const void* rpcHint) { \
        event.dispatch(*this, istream, rpcHint); \
    }

/// RPC �޽�带 ����Ѵ�
#define REGISTER_SRPC_METHOD(rpcClass, method) \
    static SRPC_RPC_EVENT(rpcClass, method) rpcClass##_##method##_event; \
    srpc::EventRegister<SRPC_RPC_EVENT(rpcClass, method), rpcClass> \
        rpcClass##_##method##_EventRegister( \
            rpcClass::SRPC_GET_RPCID(method)(), \
            &rpcClass##_##method##_event);

// = IMPLEMENT_SRPC_METHOD_DETAIL_n

/// �Ķ���Ͱ� ���� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_0(rpcClass, method, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, SRPC_TYPELIST_0() > \
            FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method); \
    void rpcClass::method(const void* rpcHint)

/// �Ķ���Ͱ� 1�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_1(rpcClass, method, \
        P1, p1, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, SRPC_TYPELIST_1(P1) > \
            FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    void rpcClass::method(const P1& p1, const void* rpcHint)

/// �Ķ���Ͱ� 2�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_2(rpcClass, method, \
        P1, p1, P2, p2, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_2(P1, P2) > FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    void rpcClass::method(const P1& p1, const P2& p2, const void* rpcHint)

/// �Ķ���Ͱ� 3�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_3(rpcClass, method, \
        P1, p1, P2, p2, P3, p3, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_3(P1, P2, P3) > FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const void* rpcHint)

/// �Ķ���Ͱ� 4�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_4(rpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_4(P1, P2, P3, P4) > FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const void* rpcHint)

/// �Ķ���Ͱ� 5�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_5(rpcClass, method, \
    P1, p1, P2, p2, P3, p3, P4, p4, \
    P5, p5, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_5(P1, P2, P3, P4, P5) > FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const void* rpcHint)

/// �Ķ���Ͱ� 6�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_6(rpcClass, method, \
        P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6) > FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const void* rpcHint)

/// �Ķ���Ͱ� 7�� �ִ� RPC Method�� �����Ѵ�
#define IMPLEMENT_SRPC_METHOD_DETAIL_7(rpcClass, method, \
    P1, p1, P2, p2, P3, p3, P4, p4, P5, p5, P6, p6, P7, p7, rpcHint) \
    class SRPC_RPC_EVENT(rpcClass, method) : public srpc::RpcEvent \
    { \
    public: \
        typedef srpc::ReceivingFunctorT<rpcClass, \
            SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7) > FunctorType; \
    public: \
        SRPC_RPC_EVENT(rpcClass, method)() : \
            srpc::RpcEvent(unmarshalFunctor_), \
            unmarshalFunctor_(&rpcClass::method) {} \
    public: \
        virtual srpc::RpcEvent* clone() const { \
            return new SRPC_RPC_EVENT(rpcClass, method); \
        } \
    private: \
        FunctorType unmarshalFunctor_; \
    }; \
    REGISTER_SRPC_METHOD(rpcClass, method) \
    void rpcClass::method(const P1& p1, const P2& p2, const P3& p3, \
        const P4& p4, const P5& p5, const P6& p6, const P7& p7, \
        const void* rpcHint)

/** @} */ // addtogroup RpcReceiving

#endif // SRPC_RPCRECEIVERDETAIL_H
