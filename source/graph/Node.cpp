// Node.cpp
#include "Node.hpp"

namespace graph {

Node::Node(EdgeId const& id, Weight const& weight) :
	_id(id),
	_weight(weight)
{}

NodeId const& Node::id() const
{
	return _id;
}

Weight const& Node::weight() const
{
	return _weight;
}

void Node::add_incident_edge(EdgeId const& edge_id)
{
	_incident_edges.push_back(edge_id);
}

void Node::add_incoming_edge(EdgeId const& edge_id)
{
	_incoming_edges.push_back(edge_id);
}

void Node::add_outgoing_edge(EdgeId const& edge_id)
{
	_outgoing_edges.push_back(edge_id);
}

void Node::add_incident_node(NodeId const& node_id)
{
	_incident_nodes.push_back(node_id);
}

void Node::add_incoming_node(NodeId const& node_id)
{
	_incoming_nodes.push_back(node_id);
}

void Node::add_outgoing_node(NodeId const& node_id)
{
	_outgoing_nodes.push_back(node_id);
}

std::size_t Node::num_incident_edges() const
{
	return _incident_edges.size();
}

std::size_t Node::num_incoming_edges() const
{
	return _incoming_edges.size();
}

std::size_t Node::num_outgoing_edges() const
{
	return _outgoing_edges.size();
}

std::size_t Node::num_incident_nodes() const
{
	return _incident_nodes.size();
}

std::size_t Node::num_incoming_nodes() const
{
	return _incoming_nodes.size();
}

std::size_t Node::num_outgoing_nodes() const
{
	return _outgoing_nodes.size();
}

std::vector<EdgeId> const& Node::incident_edges() const
{
	return _incident_edges;
}

std::vector<EdgeId> const& Node::incoming_edges() const
{
	return _incoming_edges;
}

std::vector<EdgeId> const& Node::outgoing_edges() const
{
	return _outgoing_edges;
}

std::vector<NodeId> const& Node::incident_nodes() const
{
	return _incident_nodes;
}

std::vector<NodeId> const& Node::incoming_nodes() const
{
	return _incoming_nodes;
}

std::vector<NodeId> const& Node::outgoing_nodes() const
{
	return _outgoing_nodes;
}

std::string Node::to_string() const
{
	return std::to_string(id());
}

bool Node::operator==(Node const& rhs) const
{
	return id() == rhs.id();
}

bool Node::operator!=(Node const& rhs) const
{
	return id() != rhs.id();
}

} // namespace graph
