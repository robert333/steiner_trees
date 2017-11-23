#include "Dijkstra.hpp"

namespace graph {

bool Dijkstra::is_graph_appropriated(Graph const& graph)
{
	for (Node const& node : graph.nodes()) {
		if (node.weight() < 0) {
			assert(false);
			return false;
		}
	}

	for (Edge const& edge : graph.edges()) {
		if (edge.weight() < 0) {
			assert(false);
			return false;
		}
	}

	return true;
}

Weight Dijkstra::compute_distance(Graph const& graph, Node const& source, Node const& target)
{
	std::vector<bool> visited;
	std::vector<bool> reachable;
	std::vector<Weight> length;
	std::vector<NodeId> previous;

	compute(graph, source, visited, reachable, length, previous);

	return length.at(target.id());
}

bool Dijkstra::is_reachable(Graph const& graph, Node const& source, Node const& target)
{
	std::vector<bool> visited;
	std::vector<bool> reachable;
	std::vector<Weight> length;
	std::vector<NodeId> previous;

	compute(graph, source, visited, reachable, length, previous);

	return reachable.at(target.id());
}

void Dijkstra::visit(
	Graph const& graph,
	std::vector<bool>& visited,
	std::vector<bool>& reachable,
	std::vector<Weight>& length,
	std::vector<NodeId>& previous,
	Node const& tail
)
{
	visited.at(tail.id()) = true;

	NodeId min_node_id = invalid_node_id();

	for (EdgeId const edge_id : tail.outgoing_edges()) {
		Edge const& edge = graph.edge(edge_id);
		Node const& head = graph.node(edge.head());

		if (visited.at(head.id())) {
			continue;
		}

		Weight const new_length = length.at(tail.id()) + tail.weight() + edge.weight();

		if (not reachable.at(head.id()) or length.at(head.id()) > new_length) {
			reachable.at(head.id()) = true;
			length.at(head.id()) = new_length;
			previous.at(head.id()) = tail.id();

			if (min_node_id == invalid_node_id() or length.at(min_node_id) > new_length) {
				min_node_id = head.id();
			}
		}
	}

	if (min_node_id != invalid_node_id()) {
		visit(
			graph,
			visited,
			reachable,
			length,
			previous,
			graph.node(min_node_id)
		);
		visit(
			graph,
			visited,
			reachable,
			length,
			previous,
			tail
		);
	}
}

void Dijkstra::compute(
	Graph const& graph,
	Node const& source,
	std::vector<bool>& visited,
	std::vector<bool>& reachable,
	std::vector<Weight>& length,
	std::vector<NodeId>& previous
)
{
	assert(is_graph_appropriated(graph));

	visited.assign(graph.num_nodes(), false);
	reachable.assign(graph.num_nodes(), false);
	length.assign(graph.num_nodes(), - std::numeric_limits<Weight>::infinity());
	previous.assign(graph.num_nodes(), invalid_node_id());

	reachable.at(source.id()) = true;
	length.at(source.id()) = 0;
	visited.at(source.id()) = true;

	visit(graph, visited, reachable, length, previous, source);
}

} // namespace graph
