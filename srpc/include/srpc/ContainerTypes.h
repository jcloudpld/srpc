#ifndef SRPC_CONTAINERTYPES_H
#define SRPC_CONTAINERTYPES_H

#ifdef _MSC_VER
#  pragma once
#endif

// 컨테이너 타입(STL)을 정의한다

#include "detail/Allocator.h"
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
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


/// HashMap - boost::unordered_map
template <typename Key, typename Mapped,
    typename Hash = boost::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashMap : public boost::unordered_map<Key, Mapped, Hash, Pred,
    SrpcNodeAllocator<Key> >
{};

/// HashMutliMap - boost::unordered_multimap
template <typename Key, typename Mapped,
    typename Hash = boost::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashMultiMap : public boost::unordered_multimap<Key, Mapped, Hash, Pred,
    SrpcNodeAllocator<Key> >
{};


/// HashSet - boost::unordered_set
template <typename Key,
    typename Hash = boost::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashSet :
    public boost::unordered_set<Key, Hash, Pred, SrpcNodeAllocator<Key> >
{};

/// HashMultiSet - boost::unordered_multiset
template <typename Key,
    typename Hash = boost::hash<Key>,
    typename Pred = std::equal_to<Key> >
class HashMultiSet :
    public boost::unordered_multiset<Key, Hash, Pred, SrpcNodeAllocator<Key> >
{};

/** @} */ // addtogroup types

} // namespace srpc

#endif // SRPC_CONTAINERTYPES_H
