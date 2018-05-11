// test_dijkstra_steiner_algorithm.cpp
#include "unit_tests.hpp"

#include <data/BitSetEnumerator.hpp>
#include <iostream>

TEST_CASE("BitSetEnumerator", "[BitSetEnumerator]")
{
	BitSet<10> bit_set(8);
	BitSet<10> bits_to_ignore(8);

	CHECK(bit_set.none());
	CHECK(bits_to_ignore.none());

	bits_to_ignore.set(2, true);
	bits_to_ignore.set(5, true);
	bits_to_ignore.set(6, true);

	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000000000")));
	CHECK(bits_to_ignore == BitSet<10>(8, std::bitset<10>("0001100100")));

	BitSetEnumerator<10> const bit_set_enumerator(bits_to_ignore);

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000000001")));

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000000010")));

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000000011")));

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000001000")));

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000001001")));

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000001010")));

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000001011")));

	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000010000")));


	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0010000000")));


	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	bit_set_enumerator.next(bit_set);
	std::cout << "BitSetEnumerator: bit_set = " << bit_set << "\n";
	CHECK(bit_set == BitSet<10>(8, std::bitset<10>("0000000000")));
	CHECK(bit_set.none());
}
