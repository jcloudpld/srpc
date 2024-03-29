#ifndef SRPC_UNICODE_H
#define SRPC_UNICODE_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../srpc.h"
#include "../StringTypes.h"

namespace srpc {

/// convert UCS-2(4) to UTF-8    
String SRPC_API toUtf8(const WString& ucs);

/// convert UTF-8 to UCS-2(4)    
WString SRPC_API fromUtf8(const String& utf8);

} // namespace srpc

#endif // SRPC_UNICODE_H
