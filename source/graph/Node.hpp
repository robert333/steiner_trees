// Node.hpp
#ifndef GRAPH_NODE_HPP
#define GRAPH_NODE_HPP

#include "typedefs.hpp"

namespace graph {

class Node {
public:
	using Vector = std::vector<Node>;

public:
	Node();
	
	void add_incident_edge(EdgeId const& edge_id);
	void add_incoming_edge(EdgeId const& edge_id);
	void add_outgoing_edge(EdgeId const& edge_id);

	void add_incident_node(NodeId const& node_id);
	void add_incoming_node(NodeId const& node_id);
	void add_outgoing_node(NodeId const& node_id);

	std::size_t num_incident_edges() const;
	std::size_t num_incoming_edges() const;
	std::size_t num_outgoing_edges() const;

	std::size_t num_incident_nodes() const;
	std::size_t num_incoming_nodes() const;
	std::size_t num_outgoing_nodes() const;

	std::vector<EdgeId> const& incident_edges() const;
	std::vector<EdgeId> const& incoming_edges() const;
	std::vector<EdgeId> const& outgoing_edges() const;

	std::vector<NodeId> const& incident_nodes() const;
	std::vector<NodeId> const& incoming_nodes() const;
	std::vector<NodeId> const& outgoing_nodes() const;

private:

private:
	std::vector<EdgeId> _incident_edges;
	std::vector<EdgeId> _incoming_edges;
	std::vector<EdgeId> _outgoing_edges;

	std::vector<NodeId> _incident_nodes;
	std::vector<NodeId> _incoming_nodes;
	std::vector<NodeId> _outgoing_nodes;
};

} // namespace graph

#endif
