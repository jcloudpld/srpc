#ifndef SRPC_TYPES_H
#define SRPC_TYPES_H

// 기본 데이터 타입(primitive type)을 정의한다

#include <boost/cstdint.hpp>

namespace srpc {

/** @addtogroup types
* @{
*/

// = 기본 데이터 타입 선언

typedef boost::int8_t Int8;
typedef boost::uint8_t UInt8;

typedef boost::int16_t Int16;
typedef boost::uint16_t UInt16;

typedef boost::int64_t Int64;
typedef boost::uint64_t UInt64;

typedef float Float32;
typedef double Float64;

// boost 현재 버전(1.33.1)의 integer 라이브러리는 long형이 32비트일 경우
// int32_t를 long형으로 정의하여 타입 불일치 문제를 일으키므로
// 32비트 환경에서 int를 이용하여 재정의하도록 하였음.
#if UINT_MAX == 0xffffffff
typedef int Int32;
typedef unsigned int UInt32;
#else
typedef boost::int32_t Int32;
typedef boost::uint32_t UInt32;
#endif

/** @} */ // addtogroup types

} // namespace srpc

#endif // SRPC_TYPES_H
