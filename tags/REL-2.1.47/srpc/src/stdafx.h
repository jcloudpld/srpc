#ifndef SRPC_STDAFX_H
#define SRPC_STDAFX_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <srpc/srpc.h>

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4819)
#endif
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <string>
#include <cassert>

#endif // SRPC_STDAFX_H

