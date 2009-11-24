#ifndef SRPC_RPCCONTAINERS_H
#define SRPC_RPCCONTAINERS_H

#ifdef _MSC_VER
#  pragma once
#endif

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
    void serialize(OStream& ostream) {
        ostream.write(static_cast<UInt32>(this->size()), sizeBits);
        const size_t maxNumber = MaxNumber<sizeBits>::value;
        assert(this->size() <= maxNumber);
        iterator pos = this->begin();
        const iterator end = this->end();
        for (size_t s = 0; (pos != end) && (s < maxNumber); ++pos, ++s) {
            ostream << (*pos);
        }
    }

    void serialize(IStream& istream) {
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
            istream >> rt;
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
    void serialize(OStream& ostream) {
        ostream.write(static_cast<UInt32>(this->size()), sizeBits);
        const size_t maxNumber = MaxNumber<sizeBits>::value;
        assert(this->size() <= maxNumber);
        iterator pos = this->begin();
        const iterator end = this->end();
        for (size_t s = 0; (pos != end) && (s < maxNumber); ++pos, ++s) {
            ostream << (*pos);
        }
    }

    void serialize(IStream& istream) {
        this->clear();

        UInt32 size;
        istream.read(size, sizeBits);
        if (size > MaxNumber<sizeBits>::value) {
            throw StreamingException(__FILE__, __LINE__,
                "container size is not valid");
        }
        for (UInt32 i = 0; i < size; ++i) {
            RpcType rt;
            istream >> rt;
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


/// HashSet에 대한 RpcType
template <typename RpcType>
class RHashSet : public RBaseSet<HashSet<RpcType> >
{
};

/// HashMultiSet에 대한 RpcType
template <typename RpcType>
class RHashMultiSet : public RBaseSet<HashMultiSet<RpcType> >
{
};


/// std::*map에 대한 RpcType
template <typename BaseType, typename K, typename V, size_t sizeBits>
class RBaseMap : public BaseType
{
public:
    typedef BaseType Base;
    typedef typename Base::value_type value_type;
    typedef typename Base::iterator iterator;
public:
    void serialize(OStream& ostream) {
        ostream.write(static_cast<UInt32>(this->size()), sizeBits);
        const size_t maxNumber = MaxNumber<sizeBits>::value;
        assert(this->size() <= maxNumber);
        iterator pos = this->begin();
        const iterator end = this->end();
        for (size_t s = 0; (pos != end) && (s < maxNumber); ++pos, ++s) {
            ostream << (*pos).first;
            ostream << (*pos).second;
        }
    }

    void serialize(IStream& istream) {
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
            istream >> key;
            istream >> value;
            this->insert(value_type(key, value));
        }
    }
};


/// std::map에 대한 RpcType
template <typename K, typename V, size_t sizeBits = Bits<UInt8>::size>
class RMap : public RBaseMap<Map<K, V>, K, V, sizeBits>
{
};

/// std::multimap에 대한 RpcType
template <typename K, typename V, size_t sizeBits = Bits<UInt8>::size>
class RMultiMap : public RBaseMap<MultiMap<K, V>, K, V, sizeBits>
{
};


/// HashMap에 대한 RpcType
template <typename K, typename V, size_t sizeBits = Bits<UInt8>::size>
class RHashMap : public RBaseMap<HashMap<K, V>, K, V, sizeBits>
{
};

/// HashMultiMap에 대한 RpcType
template <typename K, typename V, size_t sizeBits = Bits<UInt8>::size>
class RHashMultiMap : public RBaseMap<HashMultiMap<K, V>, K, V, sizeBits>
{
};

/** @} */ // addtogroup serialization

} // namespace srpc

#endif // SRPC_RPCCONTAINERS_H
