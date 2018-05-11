#ifndef STEINERTREES_DIJKSTRASTEINER_STEINERTREEKEY_HPP
#define STEINERTREES_DIJKSTRASTEINER_STEINERTREEKEY_HPP

#include <sstream>
#include <limits>

#include <data/BitSet.hpp>

#include <Output.hpp>

namespace steiner_trees {
namespace dijkstra_steiner {

/**
 * Class for storing the original SteinerTreeKey
 */
template<std::size_t num_terminals>
class SteinerTreeKey : public Output {
public:
	using Vertex = std::size_t;
	using Terminals = BitSet<num_terminals>;

public:
	static SteinerTreeKey<num_terminals> invalid()
	{
		return SteinerTreeKey<num_terminals>(invalid_vertex(), Terminals(0));
	}

public:
	SteinerTreeKey() :
		SteinerTreeKey(invalid())
	{}

	SteinerTreeKey(Vertex const& vertex, Terminals const& terminals) :
		_vertex(vertex),
		_terminals(terminals)
	{}

	SteinerTreeKey(Vertex const& vertex, Terminals const& terminals_1, Terminals const& terminals_2) :
		SteinerTreeKey(vertex, terminals_1 | terminals_2)
	{}

	bool is_invalid() const
	{
		return vertex() == invalid_vertex();
	}

	Vertex const& vertex() const
	{
		return _vertex;
	}

	Terminals const& terminals() const
	{
		return _terminals;
	}

	Terminals& non_const_terminals()
	{
		return _terminals;
	}

	bool operator<(SteinerTreeKey const& rhs) const
	{
		if (this->_vertex == rhs._vertex) {
			return this->_terminals < rhs._terminals;
		} else {
			return this->_vertex < rhs._vertex;
		}
	}

	virtual std::string to_string() const override
	{
		std::stringstream stringstream;
		stringstream << "(" << _vertex << ", " << _terminals << ")";
		return stringstream.str();
	}

private:
	static Vertex invalid_vertex()
	{
		return std::numeric_limits<Vertex>::max();
	}

private:
	Vertex _vertex;
	Terminals _terminals;
};

} // namespace dijkstra_steiner
} // namespace steiner_trees

#endif
