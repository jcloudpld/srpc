#ifndef NSRPC_BASEALLOCATOR_H
#define NSRPC_BASEALLOCATOR_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <ace/Malloc_Base.h>

namespace nsrpc
{

#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127)
#endif

/** @addtogroup utility
* @{
*/

/**
 * @class BaseAllocator
 *
 * ACE_Allocator의 불필요한 인터페이스 오버라이딩
 */
class BaseAllocator : public ACE_Allocator
{
public:
    virtual int remove(void) { ACE_NOTSUP_RETURN(-1); }
    virtual int bind(const char*, void*, int) { ACE_NOTSUP_RETURN(-1); }
    virtual int trybind(const char*, void*&) { ACE_NOTSUP_RETURN(-1); }
    virtual int find(const char*, void*&) { ACE_NOTSUP_RETURN(-1); }
    virtual int find(const char*) { ACE_NOTSUP_RETURN(-1); }
    virtual int unbind(const char*) { ACE_NOTSUP_RETURN(-1); }
    virtual int unbind(const char*, void*&) { ACE_NOTSUP_RETURN(-1); }
    virtual int sync(ssize_t, int) { ACE_NOTSUP_RETURN(-1); }
    virtual int sync(void*, size_t, int) { ACE_NOTSUP_RETURN(-1); }
    virtual int protect(ssize_t, int) { ACE_NOTSUP_RETURN(-1); }
    virtual int protect(void*, size_t, int) { ACE_NOTSUP_RETURN(-1); }

#if defined (ACE_HAS_MALLOC_STATS)
    virtual void print_stats(void) const {}
#endif /* ACE_HAS_MALLOC_STATS */
    virtual void dump(void) const {
#if defined (ACE_HAS_DUMP)
#endif /* ACE_HAS_DUMP */
    }
};

/** @} */ // addtogroup utility

#ifdef _MSC_VER
#  pragma warning (pop)
#endif

} // namespace nsrpc

#endif // NSRPC_BASEALLOCATOR_H
