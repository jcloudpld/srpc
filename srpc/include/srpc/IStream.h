#ifndef SRPC_ISTREAM_H
#define SRPC_ISTREAM_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "Types.h"
#include "StringTypes.h"
#include "utility/Bits.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4819)
#endif
#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/utility.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <string>
#include <cassert>

namespace srpc {

class IStream;

/** @addtogroup serialization
* @{
*/

namespace
{

/// 기본 데이터형이 아닌 것은 무조건 RpcType으로 가정
template <typename T, bool isFundamental, bool isEnum>
struct StreamReaderImpl;

// fundamental type
template <typename T>
struct StreamReaderImpl<T, true, false>
{
    static void read(IStream& istream, T& value) {
        istream.read(value);
    }
};

// enum type
template <typename T>
struct StreamReaderImpl<T, false, true>
{
    static void read(IStream& istream, T& value) {
        Int32 intValue;
        istream.read(intValue);
        value = static_cast<T>(intValue);
    }
};


template <typename T>
struct StreamReaderImpl<T, false, false>
{
    static void read(IStream& istream, T& value) {
        value.serialize(istream);
    }
};

} // namespace

/**
 * @class IStream
 *
 * Input stream.
 * - CAUTION: 데이터 타입을 기준으로 스트림에서 읽어온다
 */
class IStream : public boost::noncopyable
{
public:
    virtual ~IStream() {}

    template <typename T>
    IStream& operator>>(T& value) {
        StreamReaderImpl<T, boost::is_fundamental<T>::value,
            boost::is_enum<T>::value>::read(*this, value);
        return *this;
    }

    /// same as boost.serialization
    template <typename T>
    IStream& operator&(T& value) {
        return operator>>(value);
    }

    /// 스트림버퍼로 부터 64비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(UInt64& value, int bitCount = Bits<UInt64>::size) = 0;

    /// 스트림버퍼로 부터 32비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(UInt32& value, int bitCount = Bits<UInt32>::size) = 0;

    /// 스트림버퍼로 부터 16비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(UInt16& value, int bitCount = Bits<UInt16>::size) = 0;

    /// 스트림버퍼로 부터 8비트 부호 없는 정수 타입을 읽어 온다
    virtual void read(UInt8& value, int bitCount = Bits<UInt8>::size) = 0;

    /// 스트림버퍼로 부터 64비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(Int64& value, int bitCount = Bits<Int64>::size) = 0;

    /// 스트림버퍼로 부터 32비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(Int32& value, int bitCount = Bits<Int32>::size) = 0;

    /// 스트림버퍼로 부터 16비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(Int16& value, int bitCount = Bits<Int16>::size) = 0;

    /// 스트림버퍼로 부터 8비트 부호 있는 정수 타입을 읽어 온다
    virtual void read(Int8& value, int bitCount = Bits<Int8>::size) = 0;

    /// 스트림버퍼로 부터 부울린값을 읽어 온다
    virtual void read(bool& value, int bitCount = Bits<bool>::size) = 0;

    /// 스트림버퍼로 부터 32비트 부동 소숫점 타입을 읽어 온다
    virtual void read(Float32& value, int bitCount = Bits<Int32>::size) = 0;

    /// 최대 maxLength 길이의 문자열을 읽어온다
    virtual void read(String& value, size_t maxLength,
        int sizeBitCount) = 0;

    /// 최대 maxLength 길이의 문자열을 읽어온다
    virtual void read(WString& value, size_t maxLength,
        int sizeBitCount) = 0;

    /**
     * 스트림버퍼로 부터 raw buffer를 읽어 온다
     * @param buffer 입력 버퍼. 반드 length 이상의 메모리를 확보해야 한다.
     * @param length 읽어올 바이트 수
     */
    virtual void read(void* buffer, UInt16 length) = 0;

    /// 스트림을 바이트 단위로 정렬한다(버퍼에 남아있는 비트를 버린다)
    virtual void align() = 0;

    /**
     * 스트림을 초기화한다.
     * @param resetBuffer 버퍼를 초기화할 것인가?
     */
    virtual void reset(bool resetBuffer) = 0;

    /// 버퍼에 남아 있는 데이터의 총 크기(비트 단위)를 얻는다
    virtual int getTotalBitCount() const = 0;

    /// 버퍼에 남아 있는 데이터의 총 크기(바이트 단위)를 얻는다
    virtual int getTotalSize() const = 0;

    template <typename T>
    void do_read(T& value, int sizeBitCount, const boost::false_type&) {
        read(value, sizeBitCount);
    }

    template <typename T>
    void do_read(T& value, int sizeBitCount, const boost::true_type&) {
        Int32 temp;
        read(temp, sizeBitCount);
        value = static_cast<T>(temp);
    }
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_ISTREAM_H
