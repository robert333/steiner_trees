#ifndef STEINERTREES_DIJKSTRASTEINER_STEINERTREELABEL_HPP
#define STEINERTREES_DIJKSTRASTEINER_STEINERTREELABEL_HPP

#include "SteinerTreeKey.hpp"

namespace steiner_trees {
namespace dijkstra_steiner {

/**
 * Class for storing a Steiner Tree Label with addtional informations
 * The original Steiner Tree Label from the algorithm is implemented in SteinerTreeKey
 */
template<std::size_t num_terminals>
class SteinerTreeLabel : public Output {
public:
	using Key = SteinerTreeKey<num_terminals>;
	using Cost = graph::Weight;
	using Backtrack = std::pair<Key, Key>;

public:
	static SteinerTreeLabel<num_terminals> invalid()
	{
		return SteinerTreeLabel<num_terminals>(Key::invalid(),
											   std::numeric_limits<Cost>::max(),
											   std::numeric_limits<Cost>::max(),
											   {});
	}

public:
	SteinerTreeLabel() :
		SteinerTreeLabel(invalid())
	{}

	SteinerTreeLabel(Key const& key, Cost const& cost, Cost const& lower_bound, Backtrack const& backtrack) :
		_key(key),
		_cost(cost),
		_lower_bound(lower_bound),
		_backtrack(backtrack)
	{}

	Key const& key() const
	{
		return _key;
	}

	Cost const& cost() const
	{
		return _cost;
	}

	Cost const& lower_bound() const
	{
		return _lower_bound;
	}

	Backtrack const& backtrack() const
	{
		return _backtrack;
	}

	bool operator<(SteinerTreeLabel const& rhs) const
	{
		if (this->_cost + this->lower_bound() == rhs._cost + rhs.lower_bound()) {
			return this->_key < rhs._key;
		} else {
			return this->_cost + this->lower_bound() < rhs._cost + rhs.lower_bound();
		}
	}

	virtual std::string to_string() const override
	{
		std::stringstream stringstream;
		stringstream << key()
					 << " with cost " << cost()
					 << " and backtrack " << "{" << backtrack().first << ", " << backtrack().second << "}";
		return stringstream.str();
	}

private:
	Key _key;

	Cost _cost;
	Cost _lower_bound;

	Backtrack _backtrack;
};

} // namespace dijkstra_steiner
} // namespace steiner_trees

#endif
