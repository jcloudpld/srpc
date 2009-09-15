#ifndef VOP2PTEST_STDAFX_H
#define VOP2PTEST_STDAFX_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <svoip/config/config.h>

// OpenAL
#include "al.h"
#include "alc.h"

// ACE
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4267 4996)
#endif
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/Log_Msg.h>
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <ace/OS.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

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

#endif // VOP2PTEST_STDAFX_H
