#ifndef NSRPC_MEMORYPOOL_H
#define NSRPC_MEMORYPOOL_H

#include <ace/Guard_T.h>
#include <ace/Null_Mutex.h>
#include <boost/noncopyable.hpp>
#include <vector>
#include <queue>

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
        return static_cast<Resource*>(std::malloc(memorySize));
    }
    void free(Resource* resource) {
        std::free(resource);
    }
};


/**
 * @class MemoryPool
 *
 * Resource pool
 * - Resouce: MemoryPool���� �ٷ� ������ Ÿ��.
 * - Allocator: Resource �ν��Ͻ��� �Ҵ�/�����Ѵ�.
 *   �Ʒ��� �޽�带 �����ؾ� �Ѵ�.
 *   @code
 *     Resource* malloc();
 *     void free(Resource* resource);
 *   @endcode
 * - Mutex: Mutex Ÿ��(��, ACE_Null_Mutex, ACE_Thread_Mutex, ...)
 * - findActiveResource()�� ȿ���� �¿��Ѵ�.
 * - Effective STL Item 23 ����.
 */
template <size_t resourceSize,
    typename Resource = void,
    class Allocator = MemoryAllocator<resourceSize, Resource>,
    class Mutex = ACE_Null_Mutex>
class MemoryPool : public boost::noncopyable
{
protected:
    /// Resource pointer�� Ű������ ������������ �����Ѵ�.
    typedef std::vector<Resource*> ActiveResources;
    /// vector�� ����� �� ���� ȿ���� ����(���簡 ���� �Ҵ��� ����)
    typedef std::queue<Resource*> InactiveResources;
public:
    /**
     * ctor
     * @param poolSize Pool ũ��. ���� �� ��� poolSize��ŭ ������ ����
     *                 �Ѵ�.
     * @param allocator ���ҽ� �Ҵ�/����.
     * @param growable ���ҽ��� ���ڶ� ��� Ȯ�� �����Ѱ�?
     * @param spareSize ������ ũ��.
     */
    MemoryPool(size_t poolSize, Allocator& allocator, bool growable = true,
        size_t spareSize = 0) :
        poolSize_(poolSize),
        allocator_(allocator),
        growable_(growable),
        spareSize_(spareSize) {}

    ~MemoryPool() {
        destroy();
    }

    /// Pool�� �ʱ�ȭ�Ѵ�.
    void initialize() {
        ACE_GUARD(Mutex, monitor, lock_);

        prepareResources();
    }

    /// ���ҽ� �ν��Ͻ��� ��û�Ѵ�.
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
            throw std::bad_alloc("out of memory");
        }
        return resource;
    }

    /// ���ҽ��� �ݳ��Ѵ�.
    void release(Resource* resource) {
        ACE_GUARD(Mutex, monitor, lock_);

        typename ActiveResources::iterator pos = findActiveResource(resource);
        if ((pos != activeResources_.end()) && (*pos == resource)) {
            inactiveResources_.push(resource);
            activeResources_.erase(pos); // ����� ���� ũ��!
        }
        else {
            // �̹� �ݳ��Ǿ��ų� �߸��� �ν��Ͻ��� ���
        }
    }

    /// ��� ���ҽ��� �޸𸮸� �����Ѵ�.
    void destroy() {
        ACE_GUARD(Mutex, monitor, lock_);

        ActiveResources::iterator pos = activeResources_.begin();
        const ActiveResources::iterator end = activeResources_.end();
        for (; pos != end; ++pos) {
            allocator_.free(*pos);
        }
        activeResources_.clear();

        while (! inactiveResources_.empty()) {
            allocator_.free(inactiveResources_.front());
            inactiveResources_.pop();
        }
    }

    /// ��� ���� ���ҽ��� ���� ��´�.
    size_t getActiveResourceCount() const {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        return activeResources_.size();
    }

    /// ��� ���� ���ҽ��� ���� ��´�.
    size_t getInactiveResourceCount() const {
        ACE_GUARD_RETURN(Mutex, monitor, lock_, 0);

        return inactiveResources_.size();
    }

    size_t getPoolSize() const {
        return poolSize_;
    }
protected:
    ActiveResources& getActiveResources() {
        return activeResources_;
    }

    Mutex& getLock() {
        return lock_;
    }
private:
    void prepareResources() {
        const size_t neededSize =
            (poolSize_ + spareSize_ - inactiveResources_.size());
        for (size_t i = 0; i < neededSize; ++i) {
            inactiveResources_.push(allocator_.malloc());
        }

        if ((activeResources_.capacity() - activeResources_.size()) <
            neededSize) {
            activeResources_.reserve(neededSize);
        }
    }

    Resource* popInactiveResource() {
        Resource* resource = inactiveResources_.front();
        inactiveResources_.pop();
        return resource;
    }

    void insertActiveResource(Resource* resource) {
        const ActiveResources::iterator pos = findActiveResource(resource);
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

    mutable Mutex lock_;
};

/** @} */ // addtogroup utility

} // namespace nsrpc

#endif // NSRPC_MEMORYPOOL_H
