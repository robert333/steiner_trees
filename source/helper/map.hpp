#ifndef HELPER_MAP_HPP
#define HELPER_MAP_HPP

#include <map>

namespace helper {

template<typename Key, typename T>
bool exists(std::map<Key, T> const& map, Key const& key)
{
	return map.count(key) == 1;
}

} // namespace helper

#endif
