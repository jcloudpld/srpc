#ifndef SRPC_STRINGTYPES_H
#define SRPC_STRINGTYPES_H

#ifdef _MSC_VER
#  pragma once
#endif

// 문자열 관련 타입을 정의한다

#include "srpc.h"
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
        SrpcStringAllocator<char> >
    String;

typedef std::basic_string<wchar_t,
        std::char_traits<wchar_t>,
        SrpcStringAllocator<wchar_t> >
    WString;

// = String stream types

typedef std::basic_istringstream<char,
        std::char_traits<char>,
        SrpcStringAllocator<char> >
    IStringStream;

typedef std::basic_ostringstream<char,
        std::char_traits<char>,
        SrpcStringAllocator<char> >
    OStringStream;

typedef std::basic_istringstream<wchar_t,
        std::char_traits<wchar_t>,
        SrpcStringAllocator<wchar_t> >
    WIStringStream;

typedef std::basic_ostringstream<wchar_t,
        std::char_traits<wchar_t>,
        SrpcStringAllocator<wchar_t> >
    WOStringStream;

/// ""
SRPC_API extern const String null_string;

/// L""
SRPC_API extern const WString null_wstring;

/** @} */ // addtogroup types

} // namespace srpc

#endif // SRPC_STRINGTYPES_H
