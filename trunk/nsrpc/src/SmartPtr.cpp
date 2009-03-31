#include "stdafx.h"
#include <nsrpc/utility/SmartPtr.h>
#include <boost/interprocess/detail/atomic.hpp>

namespace nsrpc {

void MTSmartPtrTraits::addReference(const SharedObject* rawPtr)
{
    if (rawPtr != 0) {
        boost::interprocess::detail::atomic_inc32(&rawPtr->referenceCount_);
    }
}


void MTSmartPtrTraits::removeReference(SharedObject* rawPtr)
{
    if (! rawPtr) {
        return;
    }

    if (1 == boost::interprocess::detail::atomic_dec32(
        &rawPtr->referenceCount_)) {
        delete rawPtr;
    }
}


void MTSmartPtrTraits::changeReference(const SharedObject* rawPtr1,
    SharedObject* rawPtr2)
{
    MTSmartPtrTraits::addReference(rawPtr1);
    MTSmartPtrTraits::removeReference(rawPtr2);
}

} // namespace nsrpc
