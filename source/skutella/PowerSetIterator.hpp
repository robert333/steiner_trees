#ifndef SKUTELLA_POWERSETITERATOR_HPP
#define SKUTELLA_POWERSETITERATOR_HPP

#include "SkutellaProblem.hpp"

namespace skutella {

class PowerSetIterator {
public:
	using SharedPtr = std::shared_ptr<PowerSetIterator>;
	using Vector = std::vector<PowerSetIterator>;

public:
	explicit PowerSetIterator(Number set_cardinality, Number subset_cardinality);

	bool next();

	bool is_valid() const;

	std::vector<Number> compute_current_subset() const;

	std::string to_string() const;

private:
	bool is_zero_characteristic_vector();
	void next_characteristic_vector();

private:
	Number const _set_cardinality;
	Number const _subset_cardinality;

	std::vector<bool> _characteristic_vector;
};

} // namespace skutella

#endif
