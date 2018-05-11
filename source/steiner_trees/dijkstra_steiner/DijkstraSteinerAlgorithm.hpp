#ifndef STEINERTREES_DIJKSTRASTEINER_DIJKSTRASTEINERALGORITHM_HPP
#define STEINERTREES_DIJKSTRASTEINER_DIJKSTRASTEINERALGORITHM_HPP

#include <bitset>
#include <set>
#include <queue>

#include <data/BitSet.hpp>
#include <data/BitSetEnumerator.hpp>

#include <helper/vector.hpp>

#include <steiner_trees/dijkstra_steiner/SteinerTreeLabel.hpp>
#include <steiner_trees/dijkstra_steiner/SteinerTreesManager.hpp>
#include <steiner_trees/dijkstra_steiner/SteinerTreeLowerBound.hpp>

namespace steiner_trees {
namespace dijkstra_steiner {

template<typename Coord, std::size_t num_terminals>
class DijkstraSteinerAlgorithm {
public:
	using Key = SteinerTreeKey<num_terminals>;
	using Label = SteinerTreeLabel<num_terminals>;
	using Value = typename Label::Cost;
	using SinkTerminalIndex = graph::TerminalId;
	using SinkTerminalSet = std::size_t;

public:
	explicit DijkstraSteinerAlgorithm(graph::TerminalInstance const& terminal_instance) :
		_terminal_instance(terminal_instance),
		_steiner_trees_manager(terminal_instance.root_terminal_id())
	{}

	/**
	 * create all start labels, i.e. labels for the terminals
	 */
	void preparation()
	{
		for (graph::TerminalId const& sink_terminal_id : _terminal_instance.sink_terminal_ids()) {
			_sink_terminal_index_to_sink_terminal_id.push_back(sink_terminal_id);
		}

		for (SinkTerminalIndex sink_terminal_index = 0;
			 sink_terminal_index < _terminal_instance.terminals().size() - 1;
			 ++sink_terminal_index) {
			typename Key::Vertex const vertex = _terminal_instance.terminal(
				_sink_terminal_index_to_sink_terminal_id.at(sink_terminal_index)
			);
			typename Key::Terminals terminals(_terminal_instance.terminals().size() - 1);
			terminals.reset();
			assert(terminals.none());
			terminals.set(sink_terminal_index, true);

			Key const steiner_tree_key(vertex, terminals);
			Label const terminal_label(steiner_tree_key, 0, 0, {Key::invalid(), Key::invalid()});

			debug_output("terminal_label = " + terminal_label.to_string() + "\n");

			_steiner_trees_manager.update(terminal_label);
		}
	}

	/**
	 * Main Algorithm for computing an optimal Steiner Tree
	 * @return the minimum cost of such a tree
	 */
	Value run()
	{
		graph::Graph const& graph = _terminal_instance.undirected_graph();

		SteinerTreeLowerBound<Coord, num_terminals> const steiner_tree_lower_bound(
			_terminal_instance, _sink_terminal_index_to_sink_terminal_id
		);

		SteinerTreeLabel<num_terminals> label;

		while (_steiner_trees_manager.next(label)) {
			typename SteinerTreeLabel<num_terminals>::Cost const label_cost = label.cost();

			graph::Node const& node = graph.node(label.key().vertex());

			for (graph::EdgeId const& edge_id : node.incident_edges()) {
				graph::Edge const& edge = graph.edge(edge_id);
				graph::Node const& neighbour_node = graph.node(edge.opposite(node.id()));

				Key const neighbour_key(neighbour_node.id(), label.key().terminals());

				Label const neighbour_label(
					neighbour_key,
					label_cost + edge.weight(),
					steiner_tree_lower_bound.lower_bound(neighbour_key),
					{label.key(), Key::invalid()}
				);

				_steiner_trees_manager.update(neighbour_label);
			}

			BitSetEnumerator<num_terminals> const bitset_enumerator(label.key().terminals());
			Key other_key(label.key().vertex(), typename Key::Terminals(label.key().terminals().size()));

			assert(other_key.terminals().none());

			// using the bitset enumerator we check each appropriate label as required by the algorithm
			// and update it with the new cost
			while (bitset_enumerator.next(other_key.non_const_terminals())) {
				assert(other_key.terminals().any());

				if (_steiner_trees_manager.is_processed(other_key)) {
					Key const union_key(label.key().vertex(), other_key.terminals(), label.key().terminals());
					Label const union_label(union_key,
											label_cost + _steiner_trees_manager.get_minimum_cost(other_key),
											steiner_tree_lower_bound.lower_bound(union_key),
											{label.key(), other_key});

					_steiner_trees_manager.update(union_label);
				}
			}
		}

		return _steiner_trees_manager.get_minimum_cost(label.key());
	}

	/**
	 * Starting with given key, this routine computes the edges of the computed optimum Steiner Tree
	 * @return the edge set of the optimum Steiner Tree
	 */
	std::vector<graph::EdgeId> backtrack(Key const& key)
	{
		graph::Graph const& graph = _terminal_instance.undirected_graph();

		assert(not key.is_invalid());

		typename Label::Backtrack const& backtrack_item = _steiner_trees_manager.get_backtrack(key);

		debug_output(
			"backtrack key " + key.to_string() + " with cost " +
			std::to_string(_steiner_trees_manager.get_minimum_cost(key))
			+ " and backtrack {" + backtrack_item.first.to_string() + ", " +
			backtrack_item.second.to_string() + "}\n"
		);

		if (backtrack_item.first.is_invalid() and backtrack_item.second.is_invalid()) {
			return {};
		} else if (backtrack_item.second.is_invalid()) {
			std::vector<graph::EdgeId> edges = backtrack(backtrack_item.first);

			graph::EdgeId const edge_id = graph.find_edge(
				key.vertex(), backtrack_item.first.vertex()
			);

			assert(edge_id != graph::invalid_edge_id());

			edges.push_back(edge_id);

			return edges;
		} else {
			std::vector<graph::EdgeId> edges_1 = backtrack(backtrack_item.first);
			std::vector<graph::EdgeId> edges_2 = backtrack(backtrack_item.second);
			helper::append(edges_1, edges_2);
			return edges_1;
		}
	}

private:
	graph::TerminalInstance const& _terminal_instance;

	SteinerTreesManager<num_terminals> _steiner_trees_manager;

	std::vector<SinkTerminalIndex> _sink_terminal_index_to_sink_terminal_id;
};

} // namespace dijkstra_steiner
} // namespace steiner_trees

#endif
