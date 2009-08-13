#ifndef SRPC_OSTREAM_H
#define SRPC_OSTREAM_H

#include "Types.h"
#include "StringTypes.h"
#include "utility/Bits.h"
#include <boost/utility.hpp>
#include <string>
#include <cassert>

namespace srpc {

class OStream;

/** @addtogroup serialization
* @{
*/

namespace
{

/// 기본 데이터형이 아닌 것은 무조건 RpcType으로 가정
template <typename T, bool isFundamental>
struct StreamWriterImpl;


template <typename T>
struct StreamWriterImpl<T, true>
{
    static void write(OStream& ostream, T value) {
        ostream.write(value);
    }
};


template <typename T>
struct StreamWriterImpl<T, false>
{
    static void write(OStream& ostream, const T& value) {
        const_cast<boost::remove_const<T>::type&>(value).
            serialize(ostream);
    }
};

} // namespace

/**
 * @class OStream
 *
 * Output stream.
 * - CAUTION: 데이터 타입을 기준으로 스트림에 출력한다
 */
class OStream : public boost::noncopyable
{
public:
    virtual ~OStream() {}

    /// TODO: value타입에 레퍼런스를 사용하지 않을 경우 RpcType이 값으로 넘어오는
    ///       문제가 있다. 이 문제를 해결하기 위해 레퍼런스를 쓰긴 했지만,
    ///       그로인해 Primitive-type이 레퍼런스로 넘어오는 다른 문제가 생김.
    ///       최적화 방법 없나?
    template <typename T>
    OStream& operator<<(const T& value) {
        typedef boost::remove_reference<T>::type RawType;

        StreamWriterImpl<T, boost::is_fundamental<RawType>::value>::
            write(*this, value);
        return *this;
    }

    /// same as boost.serialization
    template <typename T>
    OStream& operator&(const T& value) {
        return operator<<(value);
    }

    /// for enum data type
    void write(int value, int bitCount = Bits<int>::size) {
        write(static_cast<Int32>(value), bitCount);
    }

    /// 64비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(UInt64 value, int bitCount = Bits<UInt64>::size) = 0;

    /// 32비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(UInt32 value, int bitCount = Bits<UInt32>::size) = 0;

    /// 16비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(UInt16 value, int bitCount = Bits<UInt16>::size) = 0;

    /// 8비트 부호 없는 정수를 스트림버퍼에 기록한다
    virtual void write(UInt8 value, int bitCount = Bits<UInt8>::size) = 0;

    /// 부울린 값을 스트림버퍼에 기록한다
    virtual void write(bool value, int bitCount = Bits<bool>::size) = 0;

    /**
     * 64비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(Int64 value, int bitCount = Bits<Int64>::size) = 0;

    /**
    * 32비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
    * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
    */
    virtual void write(Int32 value, int bitCount = Bits<Int32>::size) = 0;

    /**
     * 16비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(Int16 value, int bitCount = Bits<Int16>::size) = 0;

    /**
     * 8비트 부호 있는 정수 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(Int8 value, int bitCount = Bits<Int8>::size) = 0;

    /**
     * 32비트 부호 있는 부동 소수점 타입을 스트림버퍼에 기록한다
     * @param bitCount 기록할 비트 수. 2비트 이상이어야 한다
     */
    virtual void write(Float32 value, int bitCount = Bits<Int32>::size) = 0;

    /// 최대 maxLength 길이의 문자열을 스트림버퍼에 기록한다
    virtual void write(const String& value, size_t maxLength,
        int sizeBitCount) = 0;

    /// 최대 maxLength 길이의 문자열을 스트림버퍼에 기록한다
    virtual void write(const WString& value, size_t maxLength,
        int sizeBitCount) = 0;

    /**
     * raw buffer를 스트림버퍼에 기록한다
     * @param buffer raw buffer
     * @param length 버퍼의 바이트수(USHRT_MAX 보다 작거나 같아야 한다)
     */
    virtual void write(const void* buffer, UInt16 length) = 0;

    /// 패딩을 추가하여 바이트 단위로 정렬한다
    virtual void align() = 0;

    /**
     * 비트 스트림을 초기화한다
     * @param resetBuffer 버퍼를 초기화할 것인가?
     */
    virtual void reset(bool resetBuffer) = 0;

    /// 저장된 데이터의 크기(비트 단위)를 얻는다
    virtual int getTotalBitCount() const = 0;

    /// 저장된 데이터의 크기(바이트 단위)를 얻는다
    virtual int getTotalSize() const = 0;
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_OSTREAM_H
