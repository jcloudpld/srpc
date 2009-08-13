#ifndef MESSAGEBLOCKMANAGERTEST_STDAFX_H
#define MESSAGEBLOCKMANAGERTEST_STDAFX_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <nsrpc/nsrpc.h>

// ACE
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4267 4312 4996)
#endif
#include <ace/Message_Block.h>
#include <ace/Time_Value.h>
#include <ace/OS.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif

#ifdef _MSC_VER
// conditional expression is constant
#  pragma warning ( disable: 4127 )

// copy constructor could not be generated
#  pragma warning ( disable: 4511 )

// assignment operator could not be generated
#  pragma warning ( disable: 4512 )
#endif

#endif // MESSAGEBLOCKMANAGERTEST_STDAFX_H
