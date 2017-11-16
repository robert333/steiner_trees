#ifndef GRAPH_DIJKSTRA_HPP
#define GRAPH_DIJKSTRA_HPP

#include "../Graph.hpp"

namespace graph {

class Dijkstra {
public:
	static bool is_graph_appropriated(Graph const& graph);

	static Weight compute_distance(Graph const& graph, Node const& source, Node const& target);

private:
	static void visit(
		Graph const& graph,
		std::vector<bool>& visited,
		std::vector<bool>& reachable,
		std::vector<Weight>& length,
		std::vector<NodeId>& previous,
		Node const& tail
	);

	static void compute(
		Graph const& graph,
		Node const& source,
		std::vector<bool>& visited,
		std::vector<bool>& reachable,
		std::vector<Weight>& length,
		std::vector<NodeId>& previous
	);

private:
	explicit Dijkstra() = default;
};

} // namespace graph

#endif
