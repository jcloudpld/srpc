#include "stdafx.h"
#include <nsrpc/utility/SmartPtr.h>
#include <boost/detail/interlocked.hpp>

namespace nsrpc {

namespace {

template <typename T>
void AtomicIncrement(T* value)
{
#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || \
    defined( __CYGWIN__ )
    (void)BOOST_INTERLOCKED_INCREMENT(reinterpret_cast<long*>(value));
#elif defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )
    (void)boost::detail::atomic_conditional_increment(value);
#endif
}


/// @return 연산 후 (value == 0)이면 true
template <typename T>
bool AtomicDecrement(T* value)
{
#if defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || \
    defined( __CYGWIN__ )
    return BOOST_INTERLOCKED_DECREMENT(reinterpret_cast<long*>(value)) == 0;
#elif defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) )
    return boost::detail::atomic_exchange_and_add(value) == 1;
#endif
}

} // namespace

void MTSmartPtrTraits::addReference(const SharedObject* rawPtr)
{
    if (rawPtr != 0) {
        BOOST_INTERLOCKED_INCREMENT(&rawPtr->referenceCount_);
    }
}


void MTSmartPtrTraits::removeReference(SharedObject* rawPtr)
{
    if (rawPtr != 0) {
        if (BOOST_INTERLOCKED_DECREMENT(&rawPtr->referenceCount_)) {
            delete rawPtr;
        }
    }
}


void MTSmartPtrTraits::changeReference(const SharedObject* rawPtr1,
    SharedObject* rawPtr2)
{
    MTSmartPtrTraits::addReference(rawPtr1);
    MTSmartPtrTraits::removeReference(rawPtr2);
}

} // namespace nsrpc
