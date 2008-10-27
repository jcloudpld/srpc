#ifndef SRPC_TYPELIST_H
#define SRPC_TYPELIST_H

// ���� Modern C++ Designå�� ���� �� Loki ���̺귯���� �̿��߾�����,
// DRTW(Don't Reinvent The Wheel) ��Ģ�� Boost ���̺귯���� ���� Ȱ�����ڴ�
// �������� boost::mpl::vector�� ��ü�Ͽ���.
// ��������� �ҽ� ũ��� �پ��� ������ �ð��� �ð� �Ǿ���.

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
