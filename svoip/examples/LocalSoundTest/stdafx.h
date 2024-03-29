#ifndef SVOIP_STDAFX_H
#define SVOIP_STDAFX_H

#ifdef _MSC_VER
#  pragma once
#endif

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

#endif // SVOIP_STDAFX_H

