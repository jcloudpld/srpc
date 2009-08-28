#ifndef SRPC_BITS_H
#define SRPC_BITS_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <climits>
#include <cassert>

namespace srpc {

/// get bit-count of type T
template <typename T>
struct Bits
{
    static const int size = (sizeof(T) * CHAR_BIT);
};


/// get bit-count of type bool
template <>
struct Bits<bool>
{
    static const int size = 1;
};


/// ��Ʈ�� ä��� ���� ����Ʈ���� ���Ѵ�
template <typename T>
inline T toBytes(T bitCount)
{
    if (bitCount < 0) {
        return 0;
    }
    return (bitCount + (CHAR_BIT - 1)) / CHAR_BIT;    
}


/// ��Ʈ������ �� �ִ� ��Ʈ���ΰ�?
template <typename T>
inline bool isEnoughBits(int bitCount)
{
    return (0 < bitCount) && (bitCount <= Bits<T>::size);
}


/// bitCount��Ʈ ��ŭ�� ��ȣ���� ������ ǥ���� �� �ִ� �ִ밪�� ��´�.
template <size_t bitCount>
struct MaxNumber {
    enum {
        power = 2 * MaxNumber<bitCount - 1>::power,
        value = (2 * power) - 1
    };
};


template <>
struct MaxNumber<1> {
    enum {
        power = 1,
        value = 1
    };
};


template <>
struct MaxNumber<0> {
    enum {
        power = 0,
        value = 0
    };
};


/// bitCount��Ʈ ��ŭ�� ��ȣ���� ������ ǥ���� �� �ִ� �ִ밪�� ��´�.
inline size_t getMaxNumberOf(size_t bitCount)
{
    size_t maxNumber = 1;
    for (size_t b = 0; b < bitCount; ++b) {
        maxNumber *= 2;
    }
    return maxNumber - 1;
}

} // namespace srpc

#endif // SRPC_BITS_H
