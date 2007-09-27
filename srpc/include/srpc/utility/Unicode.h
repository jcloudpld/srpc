#ifndef SRPC_UNICODE_H
#define SRPC_UNICODE_H

#include "../config/config.h"
#include <string>

namespace srpc {

/// convert UCS-2(4) to UTF-8    
std::string SRPC_API toUtf8(const std::wstring& ucs);


/// convert UTF-8 to UCS-2(4)    
std::wstring SRPC_API fromUtf8(const std::string& utf8);

} // namespace srpc

#endif // SRPC_UNICODE_H
