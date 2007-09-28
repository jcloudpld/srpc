#include "stdafx.h"
#include <nsrpc/utility/SmartPtr.h>

namespace nsrpc {

void MTSmartPtrTraits::addReference(const SharedObject* rawPtr)
{
    assert(false && "TODO: atomic op 지원 ");

    if (rawPtr != 0) {
        ++rawPtr->referenceCount_;
    }
}


void MTSmartPtrTraits::removeReference(SharedObject* rawPtr)
{
    assert(false && "TODO: atomic op 지원 ");

    if (rawPtr != 0) {
        if (--rawPtr->referenceCount_ == 0) {
            delete rawPtr;
        }
    }
}


void MTSmartPtrTraits::changeReference(const SharedObject* rawPtr1,
    SharedObject* rawPtr2)
{
    assert(false && "TODO: atomic op 지원 ");

    if (rawPtr1 != 0) {
        ++rawPtr1->referenceCount_;
    }

    if (rawPtr2 != 0) {
        if (--rawPtr2->referenceCount_ == 0) {
            delete rawPtr2;
        }
    }
}

} // namespace nsrpc
