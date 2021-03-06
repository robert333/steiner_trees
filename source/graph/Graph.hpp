// Graph.hpp
#ifndef GRAPH_GRAPH_HPP
#define GRAPH_GRAPH_HPP

#include "Node.hpp"
#include "Edge.hpp"

#include "../json.hpp"

namespace graph {

class Graph {
public:

public:
	explicit Graph(bool const& directed = false);

	bool is_directed() const;

	std::size_t num_nodes() const;
	std::size_t num_edges() const;

	NodeId create_node(
		Weight const& weight = 0
	);

	EdgeId create_edge(
		NodeId const& tail,
		NodeId const& head,
		Weight const& weight = 1
	);

	Node const& node(NodeId const& node_id) const;
	Edge const& edge(EdgeId const& edge_id) const;

	Node::Vector const& nodes() const;
	Edge::Vector const& edges() const;

	EdgeId find_edge(NodeId const& tail_id, NodeId const& head_id) const;
	EdgeId find_edge(Node const& tail, Node const& head) const;
	bool exists_edge(Node const& tail, Node const& head) const;

	Graph bidirect() const;

	json export_to_json() const;

private:

private:
	bool const _directed;

	Node::Vector _nodes;
	Edge::Vector _edges;
};

} // namespace graph

#endif
