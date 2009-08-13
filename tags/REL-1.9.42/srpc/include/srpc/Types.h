#ifndef SRPC_TYPES_H
#define SRPC_TYPES_H

// �⺻ ������ Ÿ��(primitive type)�� �����Ѵ�

#include <boost/cstdint.hpp>

namespace srpc {

/** @addtogroup types
* @{
*/

// = �⺻ ������ Ÿ�� ����

typedef boost::int8_t Int8;
typedef boost::uint8_t UInt8;

typedef boost::int16_t Int16;
typedef boost::uint16_t UInt16;

typedef boost::int32_t Int32;
typedef boost::uint32_t UInt32;

typedef boost::int64_t Int64;
typedef boost::uint64_t UInt64;

typedef float Float32;
typedef double Float64;

/** @} */ // addtogroup types

} // namespace srpc

#endif // SRPC_TYPES_H
