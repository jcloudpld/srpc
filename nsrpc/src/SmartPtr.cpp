#include "stdafx.h"
#include <nsrpc/utility/SmartPtr.h>
#include <boost/detail/interlocked.hpp>

namespace nsrpc {

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
