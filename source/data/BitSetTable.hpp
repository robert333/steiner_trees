// BitSetTable.hpp
#ifndef BITSETTABLE_HPP
#define BITSETTABLE_HPP

#include <map>
#include <vector>
#include "BitSet.hpp"

template<std::size_t bitset_size, typename Value>
class BitSetTable {
public:
//	using Table = std::vector<Value>;
	using Table = std::map<std::size_t, Value>;

	BitSetTable() = default;

	Table const& table() const
	{
		return _table;
	}

	std::size_t size() const
	{
		return _table.size();
	}

	bool exists(BitSet<bitset_size> const& bit_set) const
	{
//		return bit_set.id() < size();
		return table().count(bit_set.id()) == 1;
	}

	Value const& at(BitSet<bitset_size> const& bit_set) const
	{
		assert(exists(bit_set));
		return _table.at(bit_set.id());
	}

	Value& at(BitSet<bitset_size> const& bit_set)
	{
		assert(exists(bit_set));
		return _table.at(bit_set.id());
	}

	void set(BitSet<bitset_size> const& bit_set, Value const& value)
	{
//		if (not exists(bit_set)) {
//			_table.resize(bit_set.id() + 1);
//		}

//		_table.at(bit_set.id()) = value;
		_table[bit_set.id()] = value;
	}

private:
	Table _table;
};

#endif
