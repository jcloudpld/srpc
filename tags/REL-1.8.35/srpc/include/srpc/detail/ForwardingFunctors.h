#ifndef SRPC_FORWARDINGFUNCTORS_H
#define SRPC_FORWARDINGFUNCTORS_H

#include "TypeList.h"
#include "../srpc.h"
#include <boost/utility.hpp>

namespace srpc {

class OStream;

/** @addtogroup RpcForwarding
* @{
*/

/**
 * @struct ForwardingFunctor
 *
 * forwarding functor base struct
 * - marshaling
 */
class ForwardingFunctor : public boost::noncopyable
{
public:
    ForwardingFunctor() {}
    virtual ~ForwardingFunctor() {}

    virtual void marshal(OStream& ostream) = 0;
};

// = Marshal Functors

template <typename TList>
struct ForwardingFunctorT;


/// 0 parameter
template<>
struct ForwardingFunctorT<SRPC_TYPELIST_0()> : public ForwardingFunctor
{
    virtual void marshal(OStream& /*ostream*/) {}
};


/// 1 parameter
template <typename P1>
struct ForwardingFunctorT<SRPC_TYPELIST_1(P1)> : public ForwardingFunctor
{
    ForwardingFunctorT(const P1& p1) :
        p1_(p1) {}

    virtual void marshal(OStream& ostream) {
        const_cast<P1&>(p1_).write(ostream);
    }

    // = member variable(s)
    const P1& p1_;
};


/// 2 parameter(s)
template <typename P1, typename P2>
struct ForwardingFunctorT<SRPC_TYPELIST_2(P1, P2)> : public ForwardingFunctor
{
    ForwardingFunctorT(const P1& p1, const P2& p2) :
        p1_(p1), p2_(p2) {}

    virtual void marshal(OStream& ostream) {
        const_cast<P1&>(p1_).write(ostream);
        const_cast<P2&>(p2_).write(ostream);
    }

    // = member variable(s)
    const P1& p1_;
    const P2& p2_;
};


/// 3 parameter(s)
template <typename P1, typename P2, typename P3>
struct ForwardingFunctorT<SRPC_TYPELIST_3(P1, P2, P3)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(const P1& p1, const P2& p2, const P3& p3) :
        p1_(p1), p2_(p2), p3_(p3) {}

    virtual void marshal(OStream& ostream) {
        const_cast<P1&>(p1_).write(ostream);
        const_cast<P2&>(p2_).write(ostream);
        const_cast<P3&>(p3_).write(ostream);
    }

    // = member variable(s)
    const P1& p1_;
    const P2& p2_;
    const P3& p3_;
};


/// 4 parameter(s)
template <typename P1, typename P2, typename P3, typename P4>
struct ForwardingFunctorT<SRPC_TYPELIST_4(P1, P2, P3, P4)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(const P1& p1, const P2& p2, const P3& p3,
        const P4& p4) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4) {}

    virtual void marshal(OStream& ostream) {
        const_cast<P1&>(p1_).write(ostream);
        const_cast<P2&>(p2_).write(ostream);
        const_cast<P3&>(p3_).write(ostream);
        const_cast<P4&>(p4_).write(ostream);
    }

    // = member variable(s)
    const P1& p1_;
    const P2& p2_;
    const P3& p3_;
    const P4& p4_;
};


/// 5 parameter(s)
template <typename P1, typename P2, typename P3, typename P4, typename P5>
struct ForwardingFunctorT<SRPC_TYPELIST_5(P1, P2, P3, P4, P5)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(const P1& p1, const P2& p2, const P3& p3, const P4& p4,
        const P5& p5) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5) {}

    virtual void marshal(OStream& ostream) {
        const_cast<P1&>(p1_).write(ostream);
        const_cast<P2&>(p2_).write(ostream);
        const_cast<P3&>(p3_).write(ostream);
        const_cast<P4&>(p4_).write(ostream);
        const_cast<P5&>(p5_).write(ostream);
    }

    // = member variable(s)
    const P1& p1_;
    const P2& p2_;
    const P3& p3_;
    const P4& p4_;
    const P5& p5_;
};


/// 6 parameter(s)
template <typename P1, typename P2, typename P3, typename P4, typename P5,
    typename P6>
struct ForwardingFunctorT<SRPC_TYPELIST_6(P1, P2, P3, P4, P5, P6)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(const P1& p1, const P2& p2, const P3& p3, const P4& p4,
        const P5& p5, const P6& p6) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5), p6_(p6) {}

    virtual void marshal(OStream& ostream) {
        const_cast<P1&>(p1_).write(ostream);
        const_cast<P2&>(p2_).write(ostream);
        const_cast<P3&>(p3_).write(ostream);
        const_cast<P4&>(p4_).write(ostream);
        const_cast<P5&>(p5_).write(ostream);
        const_cast<P6&>(p6_).write(ostream);
    }

    // = member variable(s)
    const P1& p1_;
    const P2& p2_;
    const P3& p3_;
    const P4& p4_;
    const P5& p5_;
    const P6& p6_;
};


/// 7 parameter(s)
template <typename P1, typename P2, typename P3, typename P4, typename P5,
    typename P6, typename P7>
struct ForwardingFunctorT<SRPC_TYPELIST_7(P1, P2, P3, P4, P5, P6, P7)> :
    public ForwardingFunctor
{
    ForwardingFunctorT(const P1& p1, const P2& p2, const P3& p3, const P4& p4,
        const P5& p5, const P6& p6, const P7& p7) :
        p1_(p1), p2_(p2), p3_(p3), p4_(p4), p5_(p5), p6_(p6), p7_(p7) {}

    virtual void marshal(OStream& ostream) {
        const_cast<P1&>(p1_).write(ostream);
        const_cast<P2&>(p2_).write(ostream);
        const_cast<P3&>(p3_).write(ostream);
        const_cast<P4&>(p4_).write(ostream);
        const_cast<P5&>(p5_).write(ostream);
        const_cast<P6&>(p6_).write(ostream);
        const_cast<P7&>(p7_).write(ostream);
    }

    // = member variable(s)
    const P1& p1_;
    const P2& p2_;
    const P3& p3_;
    const P4& p4_;
    const P5& p5_;
    const P6& p6_;
    const P7& p7_;
};

/** @} */ // addtogroup RpcForwarding

} // namespace srpc

#endif // SRPC_FORWARDINGFUNCTORS_H