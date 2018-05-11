// BitSetEnumerator.hpp
#ifndef BITSETENUMERATOR_HPP
#define BITSETENUMERATOR_HPP

#include "BitSet.hpp"

template<std::size_t bitset_size>
class BitSetEnumerator {
public:
	BitSetEnumerator(BitSet<bitset_size> const& bits_to_ignore) :
		_bits_to_ignore(bits_to_ignore)
	{}

	bool next(BitSet<bitset_size>& bit_set) const
	{
		for (std::size_t i = 0; i < bit_set.size(); ++i) {
			if (not _bits_to_ignore.at(i)) {
				bit_set.flip(i);

				if (bit_set.at(i)) {
					return true;
				}
			}
		}

		assert(bit_set.none());
		return false;
	}

private:
	BitSet<bitset_size> const _bits_to_ignore;
};

#endif
