#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <vector>
#include <memory>
#include <map>
#include <cassert>

template<typename Object, typename ... Keys>
class Storage {
public:
	using SharedPtr = std::shared_ptr<Storage>;
	using Vector = std::vector<Storage>;

private:
	using MultiKey = std::tuple<Keys ...>;

public:
	explicit Storage() = default;

	bool exists(Keys ... keys) const;

	void set(Keys ... keys, Object const& object);
	Object const& get(Keys ... keys) const;

private:
	std::map<MultiKey, Object> _storage;
};

template<typename Object, typename ... Keys>
bool Storage<Object, Keys ...>::exists(Keys ... keys) const
{
	return _storage.count(std::make_tuple(keys ...)) == 1;
}

template<typename Object, typename ... Keys>
void Storage<Object, Keys ...>::set(Keys ... keys, Object const& object)
{
	assert(not exists(keys ...));
	_storage[std::make_tuple(keys ...)] = object;
}

template<typename Object, typename ... Keys>
Object const& Storage<Object, Keys ...>::get(Keys ... keys) const
{
	return _storage.at(std::make_tuple(keys ...));
}

#endif
