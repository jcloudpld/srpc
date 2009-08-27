// stdafx.cpp : source file that includes just the standard includes
// libsrpc.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <srpc/Types.h>
#include <srpc/StringTypes.h>
#include <boost/static_assert.hpp>

BOOST_STATIC_ASSERT(1 == sizeof(srpc::Int8));
BOOST_STATIC_ASSERT(1 == sizeof(srpc::UInt8));
BOOST_STATIC_ASSERT(2 == sizeof(srpc::Int16));
BOOST_STATIC_ASSERT(2 == sizeof(srpc::UInt16));
BOOST_STATIC_ASSERT(4 == sizeof(srpc::Int32));
BOOST_STATIC_ASSERT(4 == sizeof(srpc::UInt32));
BOOST_STATIC_ASSERT(4 == sizeof(srpc::Float32));
BOOST_STATIC_ASSERT(8 == sizeof(srpc::Float64));


namespace srpc
{

const String null_string = "";

const WString null_wstring = L"";

} // namespace srpc
