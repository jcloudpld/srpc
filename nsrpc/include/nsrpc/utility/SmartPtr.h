#if !defined(NSRPC_SMARTPTR_H)
#define NSRPC_SMARTPTR_H

#include "../nsrpc.h"
#include <boost/noncopyable.hpp>
#include <cassert>

// reference count object ����� smart pointer

namespace nsrpc {

/**
* @class SharedObject
*
* reference count ����� ���̽� Ŭ����.
* - ���� ������ ��ü�� �� Ŭ�����κ��� ��ӹް� �Ҹ��ڸ� ��ȣ(protected)�Ͽ�
*   ��ü �Ҹ��� ���ƾ� �Ѵ�.
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
            "���� ������ ��ü�� �����Ҵ��ؾ� �Ѵ�");
    }
public:
    long getReferenceCount() const {
        return referenceCount_;
    }
private:
    mutable long referenceCount_;
};


/**
 * ����Ʈ SmartPtr traits.
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
 * ��Ƽ������ ����� SmartPtr traits.
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
 * ���� ��ü�� smart pointer
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
