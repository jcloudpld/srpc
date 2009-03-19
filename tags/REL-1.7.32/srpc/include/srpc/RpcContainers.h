#ifndef SRPC_RPCCONTAINERS_H
#define SRPC_RPCCONTAINERS_H

#include "ContainerTypes.h"
#include "RpcTypes.h"
#include "Exception.h"
#include "detail/Allocator.h"
#include <algorithm>
#include <functional>

namespace srpc {

/** @addtogroup serialization
* @{
*/

struct NoReservePolicy
{
    template <typename Container>
    static inline void reserve(Container& /*container*/, size_t /*size*/) {}
};


struct VectorReservePolicy
{
    template <typename Container>
    static inline void reserve(Container& container, size_t size) {
        container.reserve(size);
    }
};

/// Standard C++ Library의 Sequence Container에 대한 RpcType
template <typename SeqContainer, typename ReservePolicy, size_t sizeBits>
class RpcContainer : public SeqContainer
{
public:
    typedef SeqContainer Base;
    typedef typename Base::value_type RpcType;
    typedef typename Base::iterator iterator;
public:
    void write(OStream& ostream) {
        ostream.write(static_cast<UInt32>(this->size()), sizeBits);
        size_t maxNumber = MaxNumber<sizeBits>::value;
        iterator pos = this->begin();
        const iterator end = this->end();
        for (; (pos != end) && (maxNumber > 0); ++pos, --maxNumber) {
            (*pos).write(ostream);
        }
    }

    void read(IStream& istream) {
        this->clear();

        UInt32 size;
        istream.read(size, sizeBits);
        if (size > MaxNumber<sizeBits>::value) {
            throw StreamingException(__FILE__, __LINE__,
                "container size is not valid");
        }
        ReservePolicy::reserve(*this, size);
        for (UInt32 i = 0; i < size; ++i) {
            RpcType rt;
            rt.read(istream);
            this->push_back(rt);
        }
    }
};


/// std::vector에 대한 RpcType
template <typename RpcType, size_t sizeBits = Bits<UInt8>::size>
class RVector :
    public RpcContainer<Vector<RpcType>, VectorReservePolicy, sizeBits>
{};


/// std::list에 대한 RpcType
template <typename RpcType, size_t sizeBits = Bits<UInt8>::size>
class RList :
    public RpcContainer<List<RpcType>, NoReservePolicy, sizeBits>
{};


/// std::*set에 대한 RpcType
template <typename BaseType, size_t sizeBits = Bits<UInt8>::size>
class RBaseSet : public BaseType
{
public:
    typedef BaseType Base;
    typedef typename Base::iterator iterator;
    typedef typename Base::key_type RpcType;
public:
    void write(OStream& ostream) {
        ostream.write(static_cast<UInt32>(this->size()), sizeBits);
        size_t maxNumber = MaxNumber<sizeBits>::value;
        iterator pos = this->begin();
        const iterator end = this->end();
        for (; (pos != end) && (maxNumber > 0); ++pos, --maxNumber) {
            const_cast<RpcType&>((*pos)).write(ostream);
        }
    }

    void read(IStream& istream) {
        this->clear();

        UInt32 size;
        istream.read(size, sizeBits);
        if (size > MaxNumber<sizeBits>::value) {
            throw StreamingException(__FILE__, __LINE__,
                "container size is not valid");
        }
        for (UInt32 i = 0; i < size; ++i) {
            RpcType rt;
            rt.read(istream);
            this->insert(rt);
        }
    }
};

/// std::set에 대한 RpcType
template <typename RpcType>
class RSet : public RBaseSet<Set<RpcType> >
{
};

/// std::multiset에 대한 RpcType
template <typename RpcType>
class RMultiSet : public RBaseSet<MultiSet<RpcType> >
{
};


/// std::map에 대한 RpcType
template <typename K, typename V, size_t sizeBits = Bits<UInt8>::size>
class RMap : public Map<K, V>
{
public:
    typedef Map<K, V> Base;
    typedef typename Base::value_type value_type;
    typedef typename Base::iterator iterator;
public:
    void write(OStream& ostream) {
        ostream.write(static_cast<UInt32>(this->size()), sizeBits);
        size_t maxNumber = MaxNumber<sizeBits>::value;
        iterator pos = this->begin();
        const iterator end = this->end();
        for (; (pos != end) && (maxNumber > 0); ++pos, --maxNumber) {
            const_cast<K&>((*pos).first).write(ostream);
            const_cast<V&>((*pos).second).write(ostream);
        }
    }

    void read(IStream& istream) {
        this->clear();

        UInt32 size;
        istream.read(size, sizeBits);
        if (size > MaxNumber<sizeBits>::value) {
            throw StreamingException(__FILE__, __LINE__,
                "container size is not valid");
        }
        K key;
        V value;
        for (UInt32 i = 0; i < size; ++i) {
            key.read(istream);
            value.read(istream);
            this->insert(value_type(key, value));
        }
    }
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_RPCCONTAINERS_H
