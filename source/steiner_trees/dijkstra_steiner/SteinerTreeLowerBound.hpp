#ifndef STEINERTREES_DIJKSTRASTEINER_STEINERTREELOWERBOUND_HPP
#define STEINERTREES_DIJKSTRASTEINER_STEINERTREELOWERBOUND_HPP

#include "SteinerTreeLabel.hpp"

namespace steiner_trees {
namespace dijkstra_steiner {

template<typename Coord, std::size_t num_terminals>
class SteinerTreeLowerBound {
public:
	using Key = SteinerTreeKey<num_terminals>;
	using Label = SteinerTreeLabel<num_terminals>;
	using Vertex = typename Key::Vertex;
	using Terminals = typename Key::Terminals;
	using Cost = typename Label::Cost;

public:
	explicit SteinerTreeLowerBound(
		graph::TerminalInstance const& terminal_instance,
		std::vector<std::size_t> const& sink_terminal_index_to_sink_terminal_id
	) :
		_terminal_instance(terminal_instance),
		_sink_terminal_index_to_sink_terminal_id(sink_terminal_index_to_sink_terminal_id)
	{}

	Cost lower_bound(Key const& key) const
	{
		Cost const lower_bound_mst = minimum_spanning_tree(key);
//		std::cout << "lower_bound for " << key << " is mst = " << lower_bound_mst << "\n";
		return lower_bound_mst / 2.0;
	}

private:
	Cost minimum_spanning_tree(Key const& key) const
	{
		DUMMY_USE(key);
		return 0;
	}

private:
	graph::TerminalInstance const& _terminal_instance;
	std::vector<std::size_t> const& _sink_terminal_index_to_sink_terminal_id;
};

} // namespace dijkstra_steiner
} // namespace steiner_trees

#endif
