// Graph.cpp
#include "Graph.hpp"

namespace graph {

Graph::Graph(bool const& directed, std::size_t const& num_nodes) :
	_directed(directed),
	_weighted(true),
	_nodes(num_nodes)
{}

bool Graph::is_directed() const
{
	return _directed;
}

bool Graph::is_weighted() const
{
	return _weighted;
}

std::size_t Graph::num_nodes() const
{
	return _nodes.size();
}

std::size_t Graph::num_edges() const
{
	return _edges.size();
}

EdgeId Graph::create_edge(NodeId const& tail, NodeId const& head, Weight const& weight)
{
	assert(is_node_id_valid(tail, num_nodes()));
	assert(is_node_id_valid(head, num_nodes()));
	assert(find_edge(tail, head) == invalid_edge_id());

	if (not is_directed()) {
		assert(find_edge(tail, head) == invalid_edge_id());
	}

	EdgeId const edge_id = num_edges();

	_nodes.at(tail).add_outgoing_edge(edge_id);
	_nodes.at(head).add_incoming_edge(edge_id);
	_nodes.at(tail).add_incident_edge(edge_id);
	_nodes.at(head).add_incident_edge(edge_id);

	_edges.emplace_back(edge_id, tail, head, weight);

	return edge_id;
}

Node const& Graph::node(NodeId const& node_id) const
{
	assert(is_node_id_valid(node_id, num_nodes()));
	return _nodes[node_id];
}

Edge const& Graph::edge(EdgeId const& edge_id) const
{
	assert(is_edge_id_valid(edge_id, num_edges()));
	return _edges[edge_id];
}

Node::Vector const& Graph::nodes() const
{
	return _nodes;
}

Edge::Vector const& Graph::edges() const
{
	return _edges;
}

EdgeId Graph::find_edge(NodeId const& tail, NodeId const& head) const
{
	for (EdgeId const& edge_id : node(tail).outgoing_edges()) {
		if (head == edge(edge_id).opposite(tail)) {
			return edge_id;
		}
	}

	return invalid_edge_id();
}

Graph Graph::bidirect() const
{
	assert(not is_directed());

	Graph bidirected_graph(true, num_nodes());

	for (Edge const& edge : edges()) {
		bidirected_graph.create_edge(edge.tail(), edge.head(), edge.weight());
		bidirected_graph.create_edge(edge.head(), edge.tail(), edge.weight());
	}

	return bidirected_graph;
}

json Graph::export_to_json() const
{
	json result;

	result["directed"] = is_directed();
	result["weighted"] = is_weighted();

	for (Edge const& edge: edges()) {
		result["edges"].push_back(edge.export_to_json());
	}

	return result;
}

} // namespace graph
