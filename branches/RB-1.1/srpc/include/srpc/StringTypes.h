#ifndef SRPC_STRINGTYPES_H
#define SRPC_STRINGTYPES_H

// 문자열 관련 타입을 정의한다

#include "detail/Allocator.h"
#include <sstream>
#include <string>

namespace srpc {

/** @addtogroup types
* @{
*/

// = String types

typedef std::basic_string<char,
        std::char_traits<char>,
        SrpcNormalAllocator<char> >
    String;

typedef std::basic_string<wchar_t,
        std::char_traits<wchar_t>,
        SrpcNormalAllocator<wchar_t> >
    WString;

// = String stream types

typedef std::basic_istringstream<char,
        std::char_traits<char>,
        SrpcNormalAllocator<char> >
    IStringStream;

typedef std::basic_ostringstream<char,
        std::char_traits<char>,
        SrpcNormalAllocator<char> >
    OStringStream;

typedef std::basic_istringstream<wchar_t,
        std::char_traits<wchar_t>,
        SrpcNormalAllocator<wchar_t> >
    WIStringStream;

typedef std::basic_ostringstream<wchar_t,
        std::char_traits<wchar_t>,
        SrpcNormalAllocator<wchar_t> >
    WOStringStream;

/** @} */ // addtogroup types

} // namespace srpc

#endif // SRPC_STRINGTYPES_H
