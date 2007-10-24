#ifndef SRPC_RECEIVINGFUNCTORS_H
#define SRPC_RECEIVINGFUNCTORS_H

#include "../config/config.h"
#include "TypeList.h"
#include <boost/utility.hpp>

namespace srpc {

class IStream;

/** @addtogroup RpcReceiving
* @{
*/

/**
 * @class ReceivingFunctor
 *
 * receiving functor base class
 * - unmarshaling
 * - dispatching
 */
class ReceivingFunctor : public boost::noncopyable
{
public:
    ReceivingFunctor() {}
    virtual ~ReceivingFunctor() {}

    /// istream으로 부터 unmarshaling한다
    virtual void unmarshal(IStream& istream) = 0;

    /// functor를 호출한다
    virtual void call(void* objPtr, const void* rpcHint) = 0;
};

// = Unmarshal Functors

template <class T, typename TList>
struct ReceivingFunctorT;


/// 0 parameter
template<class T>
struct ReceivingFunctorT<T, SRPC_TYPELIST_0()> : public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& /*istream*/) {}

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
};


/// 1 parameter
template <class T, typename P1>
struct ReceivingFunctorT<T, SRPC_TYPELIST_1(P1)> : public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const P1& p1, const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& istream) {
        p1_.read(istream);
    }

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
    P1 p1_;
};


/// 2 parameter(s)
template <class T, typename P1, typename P2>
struct ReceivingFunctorT<T, SRPC_TYPELIST_2(P1, P2)> : public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const P1& p1, const P2& p2,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& istream) {
        p1_.read(istream);
        p2_.read(istream);
    }

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
};


/// 3 parameter(s)
template <class T, typename P1, typename P2, typename P3>
struct ReceivingFunctorT<T, SRPC_TYPELIST_3(P1, P2, P3)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const P1& p1, const P2& p2, const P3& p3,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& istream) {
        p1_.read(istream);
        p2_.read(istream);
        p3_.read(istream);
    }

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
};


/// 4 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4>
struct ReceivingFunctorT<T, SRPC_TYPELIST_4(P1, P2, P3, P4)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const P1& p1, const P2& p2, const P3& p3,
        const P4& p4, const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& istream) {
        p1_.read(istream);
        p2_.read(istream);
        p3_.read(istream);
        p4_.read(istream);
    }

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
};


/// 5 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4,
    typename P5>
struct ReceivingFunctorT<T, SRPC_TYPELIST_5(P1, P2, P3, P4, P5)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const P1& p1, const P2& p2, const P3& p3,
        const P4& p4, const P5& p5, const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& istream) {
        p1_.read(istream);
        p2_.read(istream);
        p3_.read(istream);
        p4_.read(istream);
        p5_.read(istream);
    }

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, p5_,
            rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
};


/// 6 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4,
    typename P5, typename P6>
struct ReceivingFunctorT<T, SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const P1& p1, const P2& p2, const P3& p3,
        const P4& p4, const P5& p5, const P6& p6, const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& istream) {
        p1_.read(istream);
        p2_.read(istream);
        p3_.read(istream);
        p4_.read(istream);
        p5_.read(istream);

        p6_.read(istream);
    }

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, p5_, p6_,
            rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
    P6 p6_;
};


/// 7 parameter(s)
template <class T, typename P1, typename P2, typename P3, typename P4,
    typename P5, typename P6, typename P7>
struct ReceivingFunctorT<T, SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)> :
    public ReceivingFunctor
{
    typedef void (T::*MemFnPtr)(const P1& p1, const P2& p2, const P3& p3,
        const P4& p4, const P5& p5, const P6& p6, const P7& p7,
        const void* rpcHint);

    ReceivingFunctorT(MemFnPtr memFnPtr) :
        memFnPtr_(memFnPtr) {}

    virtual void unmarshal(IStream& istream) {
        p1_.read(istream);
        p2_.read(istream);
        p3_.read(istream);
        p4_.read(istream);
        p5_.read(istream);
        p6_.read(istream);
        p7_.read(istream);
    }

    virtual void call(void* objPtr, const void* rpcHint) {
        (static_cast<T*>(objPtr)->*memFnPtr_)(p1_, p2_, p3_, p4_, p5_, p6_,
            p7_, rpcHint);
    }

    // = member variable(s)
    MemFnPtr memFnPtr_;
    P1 p1_;
    P2 p2_;
    P3 p3_;
    P4 p4_;
    P5 p5_;
    P6 p6_;
    P7 p7_;
};

/** @} */ // addtogroup RpcReceiving

} // namespace srpc

#endif // SRPC_RECEIVINGFUNCTORS_H
