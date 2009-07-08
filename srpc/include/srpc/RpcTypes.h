#ifndef SRPC_RPCTYPES_H
#define SRPC_RPCTYPES_H

#include "Types.h"
#include "StringTypes.h"
#include "IStream.h"
#include "OStream.h"
#include <boost/type_traits/is_enum.hpp>
#include <boost/static_assert.hpp>

namespace srpc {

/** @addtogroup serialization
* @{
*/

/// Rpc 부호 없는 숫자 타입 base template class
template <typename T, size_t bits = Bits<T>::size, typename T2 = T>
class RpcUIntType
{
public:
    typedef T NativeType;
    typedef T2 StreamingType;
public:
    RpcUIntType() {}
    RpcUIntType(const RpcUIntType& t) :
        value_(t.value_) {}
    RpcUIntType(NativeType v) :
        value_(v) {}

    void operator=(const RpcUIntType& t) {
        value_ = t.value_;
    }

    operator NativeType() const {
        return value_;
    }

    NativeType get() const {
        return value_;
    }

    void set(NativeType v) {
        value_ = v;
    }

    NativeType& ref() {
        return value_;
    }

    void write(OStream& ostream) {
        ostream.write(value_, bits);
    }

    void read(IStream& istream) {
        typedef boost::integral_constant<bool,
            ::boost::is_enum<StreamingType>::value> truth_type;
        StreamingType v;
        istream.do_read(v, bits, truth_type());
        value_ = static_cast<NativeType>(v);
    }

private:
    NativeType value_;
};


/// Rpc 부호 있는 숫자 타입 base template class
template <typename T, size_t bits = Bits<T>::size, typename T2 = T>
class RpcIntType
{
public:
    typedef T NativeType;
    typedef T2 StreamingType;
public:
    RpcIntType() {}
    RpcIntType(const RpcIntType& t) :
        value_(t.value_) {}
    RpcIntType(NativeType v) :
        value_(v) {}

    void operator=(const RpcIntType& t) {
        value_ = t.value_;
    }

    operator NativeType() const {
        return value_;
    }

    NativeType get() const {
        return value_;
    }

    void set(NativeType v) {
        value_ = v;
    }

    const NativeType& ref() const {
        return value_;
    }

    NativeType& ref() {
        return value_;
    }

    void write(OStream& ostream) {
        ostream.write(value_, bits);
    }

    void read(IStream& istream) {
        typedef boost::integral_constant<bool,
            ::boost::is_enum<StreamingType>::value> truth_type;
        StreamingType v;
        istream.do_read(v, bits, truth_type());
        value_ = static_cast<NativeType>(v);
    }

private:
    NativeType value_;
};


/**
 * Rpc String Type
 * - (maxLength >= 0) && (maxLength <= 65535)
 * - (sizeBits == 8) || (sizeBits == 16)
 * - maxLength가 256 보다 작은 경우 1바이트를 문자열 길이 필드로 사용한다.
 * - maxLength가 65536 보다 작은 경우 1바이트를 문자열 길이 필드로 사용한다.
 * - 유니코드인 경우 maxLength에 따라 sizeBits를 좀 더 정확하게 설정해야 한다.
 */
template <typename StringType, size_t maxLength,
    int sizeBits = (maxLength > UCHAR_MAX) ?
        Bits<UInt16>::size : Bits<UInt8>::size>
class RpcStringType : public StringType
{
public:
    typedef typename StringType::value_type value_type;
public:
    typedef StringType NativeType;
public:
    RpcStringType() {}
    RpcStringType(const StringType& s) :
        StringType(s) {}
    RpcStringType(const value_type* s) :
        StringType(s) {}

    void operator=(const StringType& s) {
        StringType::operator=(s);
    }

    void operator=(const value_type* s) {
        StringType::operator=(s);
    }

    const StringType& ref() const {
        return *this;
    }

    StringType& ref() {
        return *this;
    }

    void write(OStream& ostream) {
        BOOST_STATIC_ASSERT((maxLength >= 0) && (maxLength <= USHRT_MAX));
        BOOST_STATIC_ASSERT((sizeBits == 8) || (sizeBits == 16));
        ostream.write(*this, maxLength, sizeBits);
    }

    void read(IStream& istream) {
        BOOST_STATIC_ASSERT((maxLength >= 0) && (maxLength <= USHRT_MAX));
        BOOST_STATIC_ASSERT((sizeBits == 8) || (sizeBits == 16));
        istream.read(*this, maxLength, sizeBits);
    }
};


// = basic Rpc Primitive Types

typedef RpcUIntType<bool, Bits<bool>::size> RBool;

typedef RpcIntType<Int8> RInt8;
typedef RpcUIntType<UInt8> RUInt8;

typedef RpcIntType<Int16> RInt16;
typedef RpcUIntType<UInt16> RUInt16;

typedef RpcIntType<Int32> RInt32;
typedef RpcUIntType<UInt32> RUInt32;

typedef RpcIntType<Int64> RInt64;
typedef RpcUIntType<UInt64> RUInt64;

typedef RpcIntType<Float32> RFloat32;
typedef RpcIntType<Float64> RFloat64;

typedef RpcStringType<String, USHRT_MAX> RString;
typedef RpcStringType<String, UCHAR_MAX> RShortString;

typedef RpcStringType<WString, USHRT_MAX> RWString;
typedef RpcStringType<WString, UCHAR_MAX> RWShortString;

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_RPCTYPES_H
