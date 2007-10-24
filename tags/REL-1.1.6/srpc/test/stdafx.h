#ifndef SRPC_STDAFX_H
#define SRPC_STDAFX_H

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702 4312 4512)
#endif
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4511 4512 4819 4996)
#endif
#include <boost/mpl/vector.hpp>
#include <boost/type_traits.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/format.hpp>
#include <boost/utility.hpp>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <string>

#endif // SRPC_STDAFX_H
