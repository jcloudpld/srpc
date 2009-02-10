#ifndef SRPC_TYPELIST_H
#define SRPC_TYPELIST_H

// 원래 Modern C++ Design책의 예제 및 Loki 라이브러리를 이용했었지만,
// DRTW(Don't Reinvent The Wheel) 원칙과 Boost 라이브러리를 적극 활용하자는
// 취지에서 boost::mpl::vector로 대체하였다.
// 결론적으로 소스 크기는 줄었고 컴파일 시간은 늘게 되었다.

#include <boost/mpl/vector.hpp>

#define SRPC_TYPELIST_0() \
    boost::mpl::vector<>

#define SRPC_TYPELIST_1(T1) \
    boost::mpl::vector<T1>

#define SRPC_TYPELIST_2(T1, T2) \
    boost::mpl::vector<T1, T2>

#define SRPC_TYPELIST_3(T1, T2, T3) \
    boost::mpl::vector<T1, T2, T3>

#define SRPC_TYPELIST_4(T1, T2, T3, T4) \
    boost::mpl::vector<T1, T2, T3, T4>

#define SRPC_TYPELIST_5(T1, T2, T3, T4, T5) \
    boost::mpl::vector<T1, T2, T3, T4, T5>

#define SRPC_TYPELIST_6(T1, T2, T3, T4, T5, T6) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6>

#define SRPC_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7>

#define SRPC_TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7, T8>

#define SRPC_TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7, T8, T9>

#define SRPC_TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
    boost::mpl::vector<T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>

#endif // SRPC_TYPELIST_H
