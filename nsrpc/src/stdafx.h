#ifndef NSRPC_STDAFX_H
#define NSRPC_STDAFX_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>
#include <nsrpc/config/Proactor.h>

// AIO
#  ifdef _MSC_VER
#    pragma warning (push)
#    pragma warning (disable: 4127 4244 4267 4312 4996)
#  endif
#  ifdef NSRPC_USE_TPROACTOR
#    include <TProactor/Asynch_Acceptor.h>
#    include <TProactor/Asynch_Connector.h>
#    include <TProactor/Asynch_IO.h>
#    include <TProactor/Proactor.h>
#  else
#    include <ace/Asynch_Acceptor.h>
#    include <ace/Asynch_Connector.h>
#    include <ace/Asynch_IO.h>
#    include <ace/Proactor.h>
#  endif // NSRPC_USE_TPROACTOR
#  ifdef _MSC_VER
#    pragma warning (pop)
#  endif

// ACE
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4996)
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
#include <list>
#include <queue>
#include <set>
#include <vector>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <algorithm>
#include <string>
#include <cassert>

#ifdef _MSC_VER
// conditional expression is constant
#  pragma warning ( disable: 4127 )

// copy constructor could not be generated
#  pragma warning ( disable: 4511 )

// assignment operator could not be generated
#  pragma warning ( disable: 4512 )
#endif

#endif // NSRPC_STDAFX_H
