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
	explicit Graph(bool const& directed = false, std::size_t const& num_nodes = 0);

	bool is_directed() const;
	bool is_weighted() const;

	std::size_t num_nodes() const;
	std::size_t num_edges() const;

	EdgeId create_edge(
		NodeId const& tail,
		NodeId const& head,
		Weight const& weight = 1
	);

	Node const& node(NodeId const& node_id) const;
	Edge const& edge(EdgeId const& edge_id) const;

	Node::Vector const& nodes() const;
	Edge::Vector const& edges() const;

	EdgeId find_edge(NodeId const& tail, NodeId const& head) const;

	Graph bidirect() const;

	json export_to_json() const;

private:

private:
	bool const _directed;
	bool const _weighted;

	Node::Vector _nodes;
	Edge::Vector _edges;
};

} // namespace graph

#endif
