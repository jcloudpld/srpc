#ifndef SRPC_CONTAINERTYPES_H
#define SRPC_CONTAINERTYPES_H

// 컨테이너 타입(STL)을 정의한다

#include "detail/Allocator.h"
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4702)
#endif
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#ifdef _MSC_VER
#  pragma warning (pop)
#endif
#ifdef _MSC_VER
#  include <hash_map>
#  include <hash_set>
#endif //_MSC_VER

namespace srpc {

/** @addtogroup types
* @{
*/

// = Sequence containers

/// Deque - std::deque
template <typename T>
class Deque : public std::deque<T, SrpcNormalAllocator<T> >
{};

/// List - std::list
template <typename T>
class List : public std::list<T, SrpcNodeAllocator<T> >
{};

/// Vector - std::vector
template <typename T>
class Vector : public std::vector<T, SrpcNormalAllocator<T> >
{};

/// PriorityQueue - std::priority_queue
template <typename T, typename Container = Vector<T>,
    typename Compare = std::less<typename Container::value_type> >
class PriorityQueue : public std::priority_queue<T, Container, Compare>
{};

/// Queue - std::queue
template <typename T, typename Container = Deque<T> >
class Queue : public std::queue<T, Container>
{};

/// Stack - std::stack
template <typename T, typename Container = Deque<T> >
class Stack : public std::stack<T, Container>
{};

// = Associative containers

/// Map - std::map
template <typename Key, typename Type, typename Traits = std::less<Key> >
class Map : public std::map<Key, Type, Traits,
    SrpcNodeAllocator<std::pair<const Key, Type> > >
{};

/// MultiMap - std::multimap
template <typename Key, typename Type, typename Traits = std::less<Key> >
class MultiMap : public std::multimap<Key, Type, Traits,
    SrpcNodeAllocator<std::pair<const Key, Type> > >
{};

/// Set - std::set
template <typename Key, typename Traits = std::less<Key> >
class Set : public std::set<Key, Traits, SrpcNodeAllocator<Key> >
{};

/// MultiSet - std::multiset
template <typename Key, typename Traits = std::less<Key> >
class MultiSet : public std::multiset<Key, Traits, SrpcNodeAllocator<Key> >
{};

#ifdef _MSC_VER

/// HashMap - stdext::hash_map
template <typename Key, typename Type,
    typename Traits = stdext::hash_compare<Key, std::less<Key> > >
class HashMap : public stdext::hash_map<Key, Type, Traits,
    SrpcNodeAllocator<std::pair<const Key, Type> > >
{};

/// HashMutliMap - stdext::hash_multimap
template <typename Key, typename Type,
    typename Traits = stdext::hash_compare<Key, std::less<Key> > >
class HashMultiMap : public stdext::hash_multimap<Key, Type, Traits,
    SrpcNodeAllocator<std::pair<const Key, Type> > >
{};


/// HashSet - stdext::hash_set
template <typename Key,
    typename Traits = stdext::hash_compare<Key, std::less<Key> > >
class HashSet :
    public stdext::hash_set<Key, Traits, SrpcNodeAllocator<Key> >
{};

/// HashMultiSet - stdext::hash_multiset
template <typename Key,
    typename Traits = stdext::hash_compare<Key, std::less<Key> > >
class HashMultiSet :
    public stdext::hash_multiset<Key, Traits, SrpcNodeAllocator<Key> >
{};

#else

#define HashMap Map
#define HashMultiMap MultiMap

#define HashSet Set
#define HashMultiSet MultiSet

#endif //_MSC_VER

/** @} */ // addtogroup types

} // namespace srpc

#endif // SRPC_CONTAINERTYPES_H
