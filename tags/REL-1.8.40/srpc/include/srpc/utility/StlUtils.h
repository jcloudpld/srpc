#ifndef SRPC_STLUTILS_H
#define SRPC_STLUTILS_H

#include <boost/checked_delete.hpp>
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


/// 연관 컨테이너(set,map 요소의 메모리를 삭제하기 위한 functor
template<typename Pair>
inline void delete_second(Pair& pair)
{
    boost::checked_delete(pair.second);
}


/// 연관 컨테이너(map, multimap, hash_map)에서 항목을 검사한 후 값을 리턴한다
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

} // namespace srpc

#endif // SRPC_STLUTILS_H
