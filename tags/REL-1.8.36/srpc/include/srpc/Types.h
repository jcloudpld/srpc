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

typedef boost::int64_t Int64;
typedef boost::uint64_t UInt64;

typedef float Float32;
typedef double Float64;

// boost ���� ����(1.33.1)�� integer ���̺귯���� long���� 32��Ʈ�� ���
// int32_t�� long������ �����Ͽ� Ÿ�� ����ġ ������ ����Ű�Ƿ�
// 32��Ʈ ȯ�濡�� int�� �̿��Ͽ� �������ϵ��� �Ͽ���.
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
