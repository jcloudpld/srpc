#ifndef SRPC_STLUTILS_H
#define SRPC_STLUTILS_H

#ifdef _MSC_VER
#  pragma once
#endif

#include <boost/functional/hash/hash.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/checked_delete.hpp>
#include <functional>
#include <algorithm>

namespace srpc {

/// copy [src, src + size) to [dest, ...)
template<typename InIt, typename OutIt>
inline void copy(InIt src, OutIt dest, size_t size)
{
#if _MSC_VER >= 1400 // MSVC++ 8.0
    stdext::unchecked_copy(src, src + size, dest);
#else
    std::copy(src, src + size, dest);
#endif
}


template <class InputIterator, class OutputIterator, class Predicate>
inline OutputIterator copy_if(InputIterator first, InputIterator last,
    OutputIterator result, Predicate pred)
{
    for (; first != last; ++first) {
        if (pred(*first)) {
            *result++ = *first;
        }
    }
}


/// ���� �����̳�(set,map ����� �޸𸮸� �����ϱ� ���� functor
template<typename Pair>
inline void delete_second(Pair& pair)
{
    boost::checked_delete(pair.second);
}


/// ���� �����̳�(map, multimap, hash_map)���� �׸��� �˻��� �� ���� �����Ѵ�
template <class Map>
inline typename Map::value_type::second_type search_map(const Map& the_map,
    const typename Map::value_type::first_type& key,
    const typename Map::value_type::second_type& null_result =
        Map::value_type::second_type())
{
    typename Map::const_iterator pos = the_map.find(key);
    if (pos != the_map.end()) {
        return (*pos).second;
    }
    return null_result;
}


/// ��ҹ��� �������� �ʴ� equal_to (���ڿ� ����)
template <typename T>
struct iequal_to : public std::binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const {
        return boost::algorithm::iequals(x, y, std::locale());
    }
};


/// ��ҹ��� �������� �ʴ� hash (���ڿ� ����)
template <typename T>
struct ihash : public std::unary_function<T, std::size_t>
{
    std::size_t operator()(const T& x) const {
        std::size_t seed = 0;
        std::locale locale;

        T::const_iterator pos = x.begin();
        const T::const_iterator end = x.end();
        for(; pos != end; ++pos) {
            boost::hash_combine(seed, std::toupper(*pos, locale));
        }

        return seed;
    }
};

} // namespace srpc

#endif // SRPC_STLUTILS_H
