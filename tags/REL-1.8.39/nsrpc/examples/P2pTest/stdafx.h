#ifndef P2PTEST_STDAFX_H
#define P2PTEST_STDAFX_H

#include <nsrpc/nsrpc.h>

// boost
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

// crt
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <set>
#include <vector>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <queue>
#include <algorithm>
#include <string>
#include <iostream>
#include <cassert>

#endif // P2PTEST_STDAFX_H
