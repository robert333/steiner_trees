// BitSet.hpp
#ifndef BITSET_HPP
#define BITSET_HPP

#include <bitset>
#include <cassert>

template<std::size_t bitset_size>
class BitSet {
public:
	explicit BitSet(std::size_t const real_size = bitset_size) :
		_real_size(real_size)
	{ assert(_real_size <= bitset_size); }

	explicit BitSet(std::size_t const real_size, std::bitset<bitset_size> const& bitset) :
		_bitset(bitset),
		_real_size(real_size)
	{ assert(_real_size <= bitset_size); }

	std::size_t size() const
	{
		return _real_size;
	}

	void set(std::size_t const index, bool const value)
	{
		assert(index < size());
		_bitset[index] = value;
	}

	bool at(std::size_t const index) const
	{
		assert(index < size());
		return _bitset.test(index);
	}

	std::size_t id() const
	{
		return _bitset.to_ulong();
	}

	bool none() const
	{
		return _bitset.none();
	}

	bool any() const
	{
		return _bitset.any();
	}

	void reset()
	{
		_bitset.reset();
	}

	std::size_t num_ones() const
	{
		return _bitset.count();
	}

	void flip(std::size_t const index)
	{
		assert(index < size());
		_bitset.flip(index);
	}

	BitSet<bitset_size>& flip()
	{
		for (std::size_t i = 0; i < size(); ++i) {
			_bitset.flip(i);
		}
		return *this;
	}

	BitSet<bitset_size>& operator++()
	{
		for (std::size_t i = 0; i < size(); ++i) {
			if (_bitset.flip(i).test(i)) {
				return *this;
			}
		}

		_bitset.reset();
		return *this;
	}

	bool operator==(BitSet<bitset_size> const& rhs) const
	{
		return size() == rhs.size() and _bitset == rhs._bitset;
	}

	bool operator<(BitSet<bitset_size> const& rhs) const
	{
		assert(this->_real_size == rhs._real_size);
		return this->_bitset.to_ulong() < rhs._bitset.to_ulong();
	}

	BitSet<bitset_size> operator|(BitSet<bitset_size> const& rhs) const
	{
		assert(size() == rhs.size());
		return BitSet(size(), _bitset | rhs._bitset);
	}

private:
	std::bitset<bitset_size> _bitset;
	std::size_t _real_size;
};

template<std::size_t bitset_size>
std::ostream& operator<<(std::ostream& ostream, BitSet<bitset_size> const& bitset)
{
	ostream << "(";

	if (bitset.size() > 0) {
		ostream << bitset.at(0);

		for (std::size_t i = 1; i < bitset.size(); ++i) {
			ostream << ", " << bitset.at(i);
		}
	}

	ostream << ")";

	return ostream;
}

#endif
