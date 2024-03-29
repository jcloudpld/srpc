#if !defined(NSRPC_SMARTPTR_H)
#define NSRPC_SMARTPTR_H

#ifdef _MSC_VER
#  pragma once
#endif

#include "../nsrpc.h"
#include <boost/cstdint.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

// reference count object 기반의 smart pointer

namespace nsrpc {

/**
* @class SharedObject
*
* reference count 기반의 베이스 클래스.
* - 공유 가능한 객체는 이 클래스로부터 상속받고 소멸자를 보호(protected)하여
*   객체 소멸을 막아야 한다.
*/
class SharedObject : public boost::noncopyable
{
private:
    friend class SmartPtrTraits;
    friend class MTSmartPtrTraits;
protected:
    SharedObject() :
        referenceCount_(0) {}
    virtual ~SharedObject() {
        assert((referenceCount_ == 0) &&
            "Shared object must be allocated dynamically.");
    }
public:
    long getReferenceCount() const {
        return referenceCount_;
    }
private:
    mutable boost::uint32_t referenceCount_;
};


/**
 * 디폴트 SmartPtr traits.
 */
class SmartPtrTraits
{
public:
    static void addReference(const SharedObject* rawPtr) {
        if (rawPtr != 0) {
            rawPtr->referenceCount_++;
        }
    }

    static void removeReference(SharedObject* rawPtr) {
        if ((rawPtr != 0) && (--rawPtr->referenceCount_ == 0)) {
            delete rawPtr;
        }
    }

    static void changeReference(const SharedObject* rawPtr1,
            SharedObject* rawPtr2) {
        addReference(rawPtr1);
        removeReference(rawPtr2);
    }
};


/**
 * 멀티쓰레드 기반의 SmartPtr traits.
 */
class NSRPC_API MTSmartPtrTraits
{
public:
    static void addReference(const SharedObject* rawPtr);
    static void removeReference(SharedObject* rawPtr);
    static void changeReference(const SharedObject* rawPtr1,
        SharedObject* rawPtr2);
};


/**
 * @class SmartPtr
 *
 * 공유 객체의 smart pointer
 */
template <class Type, class Traits = SmartPtrTraits>
class SmartPtr
{
private:
    typedef SmartPtr<Type, Traits> ThisSmartPtr;
public:
    SmartPtr(Type* ptr = 0) {
        rawPtr_ = ptr;
        Traits::addReference(rawPtr_);
    }

    SmartPtr(const ThisSmartPtr& r) {
        rawPtr_ = r.rawPtr_;
        Traits::addReference(rawPtr_);
    }

    ~SmartPtr() {
        Traits::removeReference(rawPtr_);
    }
public:
    ThisSmartPtr& operator=(const Type* ptr) {
        Traits::changeReference(ptr, rawPtr_);
        rawPtr_ = const_cast<Type*>(ptr);
        return *this;
    }

    ThisSmartPtr& operator=(const ThisSmartPtr& r) {
        return operator=(r.rawPtr_);
    }
public:
    Type& operator*() const {
        assert(rawPtr_);
        return *rawPtr_;
    }

    Type* operator->() const {
        assert(rawPtr_);
        return rawPtr_;
    }

    Type* get() const {
        return rawPtr_;
    }
public:
    void reset() {
        Traits::removeReference(rawPtr_);
        rawPtr_ = 0;
    }

    bool isNull() const {
        return rawPtr_ == 0;
    }

    void swap(ThisSmartPtr& r) {
        Type* oldPtr;
        oldPtr = rawPtr_;
        rawPtr_ = r.rawPtr_;
        r.rawPtr_ = oldPtr;
    }
public:
    friend bool operator==(const ThisSmartPtr& r, const Type* ptr) {
        return r.rawPtr_ == ptr;
    }

    friend bool operator!=(const ThisSmartPtr& r, const Type* ptr) {
        return ! operator==(r, ptr);
    }

    friend bool operator==(const Type* ptr, const ThisSmartPtr& r) {
        return r.rawPtr_ == ptr;
    }

    friend bool operator!=(const Type* ptr, const ThisSmartPtr& r) {
        return ! operator==(ptr, r);
    }

    friend bool operator==(const ThisSmartPtr& r1, const ThisSmartPtr& r2) {
        return r1.rawPtr_ == r2.rawPtr_;
    }

    friend bool operator!=(const ThisSmartPtr& r1, const ThisSmartPtr& r2) {
        return ! operator==(r1, r2);
    }
private:
    Type* rawPtr_;
};

} // namespace nsrpc

#endif // !defined(NSRPC_SMARTPTR_H)
