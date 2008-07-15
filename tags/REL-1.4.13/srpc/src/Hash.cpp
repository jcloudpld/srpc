#include "stdafx.h"
#include <srpc/utility/Hash.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4244)
#endif
#include <boost/functional/hash/hash.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

namespace srpc {

UInt32 hash(const char* str, size_t len)
{
    return static_cast<UInt32>(boost::hash_range(str, str + len));
}

} // namespace srpc
