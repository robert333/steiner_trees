// Graph.cpp
#include "Graph.hpp"

namespace graph {

Graph::Graph(bool const& directed) :
	_directed(directed)
{}

bool Graph::is_directed() const
{
	return _directed;
}

std::size_t Graph::num_nodes() const
{
	return _nodes.size();
}

std::size_t Graph::num_edges() const
{
	return _edges.size();
}

NodeId Graph::create_node(Weight const& weight)
{
	NodeId const node_id = num_nodes();

	_nodes.emplace_back(node_id, weight);

	return node_id;
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

EdgeId Graph::find_edge(NodeId const& tail_id, NodeId const& head_id) const
{
	std::vector<EdgeId> const& edge_ids = is_directed()
										  ? node(tail_id).outgoing_edges()
										  : node(tail_id).incident_edges();

	for (EdgeId const& edge_id : edge_ids) {
		if (head_id == edge(edge_id).opposite(tail_id)) {
			return edge_id;
		}
	}

	return invalid_edge_id();
}

EdgeId Graph::find_edge(Node const& tail, Node const& head) const
{
	return find_edge(tail.id(), head.id());
}

bool Graph::exists_edge(Node const& tail, Node const& head) const
{
	return find_edge(tail, head) != invalid_edge_id();
}

Graph Graph::bidirect() const
{
	assert(not is_directed());

	Graph bidirected_graph(true);

	for (Node const& node : nodes()) {
		NodeId const node_id = bidirected_graph.create_node(node.weight());
		assert(node_id == node.id());
		DUMMY_USE(node_id);
	}

	for (Edge const& edge : edges()) {
		EdgeId const forward_edge_id = bidirected_graph.create_edge(edge.tail(), edge.head(), edge.weight());
		EdgeId const backward_edge_id = bidirected_graph.create_edge(edge.head(), edge.tail(), edge.weight());
		assert(forward_edge_id == 2 * edge.id());
		assert(backward_edge_id == 2 * edge.id() + 1);
		DUMMY_USE(forward_edge_id);
		DUMMY_USE(backward_edge_id);
	}

	return bidirected_graph;
}

json Graph::export_to_json() const
{
	json result;

	result["directed"] = is_directed();

	for (Node const& node : nodes()) {
		result["edges"].push_back(node.export_to_json());
	}

	for (Edge const& edge : edges()) {
		result["edges"].push_back(edge.export_to_json());
	}

	return result;
}

} // namespace graph
