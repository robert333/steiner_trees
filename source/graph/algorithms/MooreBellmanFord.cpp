#include "MooreBellmanFord.hpp"

#include <queue>

namespace graph {

Weight MooreBellmanFord::compute_longest_path(Graph const& graph, Node const& source, Node const& target)
{
	std::vector<Weight> length;

	std::deque<NodeId> const negative_cycle = compute(graph, source, length);

	assert(negative_cycle.empty());

	return -1 * length.at(target.id());
}

std::deque<NodeId> MooreBellmanFord::compute(Graph const& graph, Node const& source, std::vector<Weight>& length)
{
	std::vector<char> is_reachable(graph.num_nodes(), false);

	std::array<std::queue<NodeId>, 2> changed;
	std::array<std::vector<char>, 2> is_in_changed;

	is_in_changed.at(0).assign(graph.num_nodes(), false);
	is_in_changed.at(1).assign(graph.num_nodes(), false);

	std::vector<NodeId> previous(graph.num_nodes(), invalid_node_id());
	std::deque<NodeId> negative_circle;

	length.assign(graph.num_nodes(), infinity());

	length.at(source.id()) = 0;
	is_reachable.at(source.id()) = true;

	changed.at(0).push(source.id());
	is_in_changed.at(1).at(source.id()) = true;

	for (std::size_t i = 0; i < graph.num_nodes() - 1; ++i) {
		while (not changed.at(i % 2).empty()) {
			Node const& tail = graph.node(changed.at(i % 2).front());

			changed.at(i % 2).pop();
			is_in_changed.at((i + 1) % 2).at(tail.id()) = false;

			for (EdgeId const edge_id : tail.outgoing_edges()) {
				Edge const& edge = graph.edge(edge_id);
				Node const& head = graph.node(edge.head());

				Weight const new_length = length.at(tail.id()) - tail.weight() - edge.weight();

				if (not is_reachable.at(head.id()) or length.at(head.id()) > new_length) {
					is_reachable.at(head.id()) = true;
					length.at(head.id()) = new_length;
					previous.at(head.id()) = tail.id();

					if (not is_in_changed.at(i % 2).at(head.id())) {
						changed.at((i + 1) % 2).push(head.id());
						is_in_changed.at(i % 2).at(head.id()) = true;
					}
				}
			}
		}
	}

	// search for negative circle
	while (not changed.at((graph.num_nodes() - 1) % 2).empty() and negative_circle.empty()) {
		Node const& tail = graph.node(changed.at((graph.num_nodes() - 1) % 2).front());

		changed.at((graph.num_nodes() - 1) % 2).pop();

		for (EdgeId const edge_id : tail.outgoing_edges()) {
			Edge const& edge = graph.edge(edge_id);
			Node const& head = graph.node(edge.head());

			if (length.at(head.id()) > length.at(tail.id()) + edge.weight()) {
				std::vector<bool> visited(graph.num_nodes(), false);

				visited.at(tail.id()) = true;
				visited.at(head.id()) = true;

				NodeId t = previous.at(tail.id());

				while (not visited.at(t)) {
					visited.at(t) = true;
					t = previous.at(t);
				}

				negative_circle.push_front(t);
				for (NodeId x = previous.at(t); x != t; x = previous.at(x)) {
					negative_circle.push_front(x);
				}

				break;
			}
		}
	}

	return negative_circle;
}

} // namespace graph
