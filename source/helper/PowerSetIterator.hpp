#ifndef HELPER_POWERSETITERATOR_HPP
#define HELPER_POWERSETITERATOR_HPP

#include <memory>
#include <vector>

namespace helper {

class PowerSetIterator {
public:
	using SharedPtr = std::shared_ptr<PowerSetIterator>;
	using Vector = std::vector<PowerSetIterator>;
	using Cardinality = std::size_t;
	using Set = std::vector<Cardinality>;

public:
	explicit PowerSetIterator(Cardinality set_cardinality, Cardinality subset_cardinality);

	bool next();

	bool is_valid() const;

	Set compute_current_subset() const;

	std::string to_string() const;

private:
	bool is_zero_characteristic_vector();
	void next_characteristic_vector();

private:
	Cardinality const _set_cardinality;
	Cardinality const _subset_cardinality;

	std::vector<bool> _characteristic_vector;
};

} // namespace helper

#endif
