#ifndef NSRPC_MEMORYPOOL_H
#define NSRPC_MEMORYPOOL_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <srpc/ContainerTypes.h>
#include <srpc/Types.h>
#include <ace/Guard_T.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4127 4244 4267 4312 4996)
#endif
#include <ace/Null_Mutex.h>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#include <boost/noncopyable.hpp>

namespace nsrpc
{

/** @addtogroup utility
* @{
*/

/**
 * @class MemoryAllocator
 */
template <size_t memorySize, typename Resource = void>
class MemoryAllocator
{
public:
    Resource* malloc() {
        return static_cast<Resource*>(new srpc::UInt8[memorySize]);
    }
    void free(void* resource) {
        delete[] resource;
    }
};


/**
 * @class MemoryPoolCallback
 */
class MemoryPoolCallback
{
public:
    virtual ~MemoryPoolCallback() {}

    virtual void poolGrowing(size_t neededSize) = 0;

    virtual void poolGrowed(size_t activeResourceSize,
        size_t inactiveResourceSize) = 0;
};


/**
 * @class MemoryPool
 *
 * Resource pool
 * - Resouce: MemoryPool에서 다룰 데이터 타입.
 * - Allocator: Resource 인스턴스를 할당/해제한다.
 *   아래의 메쏘드를 지원해야 한다.
 *   @code
 *     Resource* malloc();
 *     void free(Resource* resource);
 *   @endcode
 * - Mutex: Mutex 타입(예, ACE_Null_Mutex, ACE_Thread_Mutex, ...)
 * - findActiveResource()이 효율을 좌우한다.
 * - Effective STL Item 23 참고.
 */
template <size_t resourceSize,
    typename Resource = void,
    class Allocator = MemoryAllocator<resourceSize, Resource>,
    class Mutex = ACE_Null_Mutex>
class MemoryPool : public boost::noncopyable
{
protected:
    /// Resource pointer를 키값으로 오름차순으로 정렬한다.
    typedef srpc::Vector<Resource*> ActiveResources;
    /// vector를 사용한 것 보다 효율이 좋다(복사가 적고 할당이 많다)
    typedef srpc::Queue<Resource*> InactiveResources;
public:
    /**
     * ctor
     * @param poolSize Pool 크기. 가득 찰 경우 poolSize만큼 세션을 생성
     *                 한다.
     * @param allocator 리소스 할당/해제.
     * @param growable 리소스가 모자랄 경우 확장 가능한가?
     * @param spareSize 여유분 크기.
     */
    MemoryPool(size_t poolSize, Allocator& allocator, bool growable = true,
        size_t spareSize = 0, MemoryPoolCallback* callback = 0) :
        poolSize_(poolSize),
        allocator_(allocator),
        growable_(growable),
        spareSize_(spareSize),
        callback_(callback) {}

    ~MemoryPool() {
        destroy();
    }

    /// Pool을 초기화한다.
    void initialize() {
        ACE_GUARD(Mutex, monitor, lock_);

        prepareResources();
    }

    /// 리소스 인스턴스를 요청한다.
    Resource* acquire() {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        if (isFull()) {
            if (! growable_) {
                return 0;
            }
            prepareResources();
        }

        Resource* resource = 0;
        if (! inactiveResources_.empty()) {
            resource = popInactiveResource();
            insertActiveResource(resource);
        }
        else {
            throw std::bad_alloc();
        }
        return resource;
    }

    /// 리소스를 반납한다.
    void release(Resource* resource) {
        ACE_GUARD(Mutex, monitor, lock_);

        const typename ActiveResources::iterator pos =
            findActiveResource(resource);
        if ((pos != activeResources_.end()) && (*pos == resource)) {
            inactiveResources_.push(resource);
            activeResources_.erase(pos); // 비용이 제일 크다!
        }
        else {
            // 이미 반납되었거나 잘못된 인스턴스인 경우
        }
    }

    /// 모든 리소스의 메모리를 해제한다.
    void destroy() {
        ACE_GUARD(Mutex, monitor, lock_);

        typename ActiveResources::iterator pos = activeResources_.begin();
        const typename ActiveResources::iterator end = activeResources_.end();
        for (; pos != end; ++pos) {
            allocator_.free(*pos);
        }
        activeResources_.clear();

        while (! inactiveResources_.empty()) {
            allocator_.free(inactiveResources_.front());
            inactiveResources_.pop();
        }
    }

    template <typename Functor>
    void forEachActiveResources(Functor func) {
        ACE_GUARD(Mutex, monitor, lock_);

        // func를 호출하는 동안 activeResources_가 변경될 수 있으므로 안전하게
        // 복사한다
        ActiveResources activeResources = activeResources_;
        std::for_each(activeResources.begin(), activeResources.end(), func);
    }

    /// 사용 중인 리소스의 수를 얻는다.
    size_t getActiveResourceCount() const {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        return activeResources_.size();
    }

    /// 대기 중인 리소스의 수를 얻는다.
    size_t getInactiveResourceCount() const {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        return inactiveResources_.size();
    }

    size_t getPoolSize() const {
        return poolSize_;
    }
private:
    void prepareResources() {
        const size_t neededSize =
            (poolSize_ + spareSize_ - inactiveResources_.size());

        if (callback_ != 0) {
            callback_->poolGrowing(neededSize);
        }

        for (size_t i = 0; i < neededSize; ++i) {
            inactiveResources_.push(allocator_.malloc());
        }

        if ((activeResources_.capacity() - activeResources_.size()) <
            neededSize) {
            activeResources_.reserve(neededSize);
        }

        if (callback_ != 0) {
            callback_->poolGrowed(activeResources_.size(),
                inactiveResources_.size());
        }
    }

    Resource* popInactiveResource() {
        Resource* resource = inactiveResources_.front();
        inactiveResources_.pop();
        return resource;
    }

    void insertActiveResource(Resource* resource) {
        const typename ActiveResources::iterator pos =
            findActiveResource(resource);
        if (pos != activeResources_.end()) {
            assert(*pos != resource);
            activeResources_.insert(pos, resource);
        }
        else {
            activeResources_.push_back(resource);
        }
    }

    typename ActiveResources::iterator findActiveResource(Resource* resource) {
        return std::lower_bound(activeResources_.begin(),
            activeResources_.end(), resource);
    }

    bool isFull() const {
        return inactiveResources_.size() <= spareSize_;
    }
private:
    size_t poolSize_;
    Allocator& allocator_;
    bool growable_;
    size_t spareSize_;

    ActiveResources activeResources_;
    InactiveResources inactiveResources_;

    MemoryPoolCallback* callback_;

    mutable Mutex lock_;
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_MEMORYPOOL_H
