#include "SteinerTreeCut.hpp"

namespace steiner_trees {

SteinerTreeCut::SteinerTreeCut(graph::Graph const& graph, graph::Net const& net) :
	_graph(graph),
	_net(net),
	_characteristic_vector(graph.num_nodes(), false)
{
	while (not is_valid()) {
		next_characteristic_vector();
	}
}

bool SteinerTreeCut::next()
{
	do {
		next_characteristic_vector();
	} while (not is_valid() and not is_zero_characteristic_vector());

	return not is_zero_characteristic_vector();
}

bool SteinerTreeCut::is_valid() const
{
	std::vector<graph::NodeId> const nodes_within_cut = compute_nodes_within_cut();

	return not helper::contains(nodes_within_cut, _net.terminal(0))
		   and not helper::are_disjoint(nodes_within_cut, _net.terminals());
}

std::vector<graph::NodeId> SteinerTreeCut::compute_nodes_within_cut() const
{
	std::vector<graph::NodeId> nodes_within_cut;

	for (graph::NodeId node_id = 0; node_id < _graph.num_nodes(); ++node_id) {
		if (_characteristic_vector.at(node_id)) {
			nodes_within_cut.push_back(node_id);
		}
	}

	return nodes_within_cut;
}

std::vector<graph::EdgeId> SteinerTreeCut::compute_outgoing_edges() const
{
	assert(is_valid());

	std::vector<graph::NodeId> const nodes_within_cut = compute_nodes_within_cut();

	std::vector<graph::EdgeId> outgoing_edges;

	for (graph::NodeId const& node_id : nodes_within_cut) {
		graph::Node const& node = _graph.node(node_id);

		for (graph::EdgeId const& edge_id : _graph.is_directed() ? node.outgoing_edges() : node.incident_edges()) {
			graph::Edge const& edge = _graph.edge(edge_id);

			if (not helper::contains(nodes_within_cut, edge.opposite(node_id))) {
				outgoing_edges.push_back(edge_id);
			}
		}
	}

	return outgoing_edges;
}

std::string SteinerTreeCut::to_string() const
{
	return helper::to_string(compute_nodes_within_cut(), "nodes_within_cut");
}

void SteinerTreeCut::next_characteristic_vector()
{
	for (std::size_t i = 0; i < _characteristic_vector.size(); ++i) {
		_characteristic_vector.at(i) = not _characteristic_vector.at(i);

		if (_characteristic_vector.at(i)) {
			break;
		}
	}
}

bool SteinerTreeCut::is_zero_characteristic_vector()
{
	for (std::size_t i = 0; i < _characteristic_vector.size(); ++i) {
		if (_characteristic_vector.at(i)) {
			return false;
		}
	}

	return true;
}

} // namespace steiner_trees
