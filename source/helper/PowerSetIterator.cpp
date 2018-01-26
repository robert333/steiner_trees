#include "PowerSetIterator.hpp"
#include "../helper/vector.hpp"

namespace helper {

PowerSetIterator::PowerSetIterator(Cardinality set_cardinality, Cardinality subset_cardinality) :
	_set_cardinality(set_cardinality),
	_subset_cardinality(subset_cardinality),
	_characteristic_vector(set_cardinality, false)
{
	while (not is_valid()) {
		next_characteristic_vector();
	}
}

bool PowerSetIterator::next()
{
	do {
		next_characteristic_vector();
	} while (not is_valid() and not is_zero_characteristic_vector());

	return not is_zero_characteristic_vector();
}

bool PowerSetIterator::is_valid() const
{
	std::size_t cardinality = 0;

	for (bool is_contained : _characteristic_vector) {
		if (is_contained) {
			++cardinality;
		}
	}

	return cardinality == _subset_cardinality;
}

PowerSetIterator::Set PowerSetIterator::compute_current_subset() const
{
	Set subset;

	for (std::size_t i = 0; i < _characteristic_vector.size(); ++i) {
		if (_characteristic_vector.at(i)) {
			subset.push_back(i);
		}
	}

	return subset;
}

std::string PowerSetIterator::to_string() const
{
	return helper::to_string(compute_current_subset(), "subset");
}

void PowerSetIterator::next_characteristic_vector()
{
	for (std::size_t i = 0; i < _characteristic_vector.size(); ++i) {
		_characteristic_vector.at(i) = not _characteristic_vector.at(i);

		if (_characteristic_vector.at(i)) {
			break;
		}
	}
}

bool PowerSetIterator::is_zero_characteristic_vector()
{
	for (bool is_contained : _characteristic_vector) {
		if (is_contained) {
			return false;
		}
	}

	return true;
}

} // namespace helper
