// Edge.hpp
#ifndef GRAPH_EDGE_HPP
#define GRAPH_EDGE_HPP

#include "typedefs.hpp"
#include "../json.hpp"

namespace graph {

class Edge : public Output {
public:
	using Vector = std::vector<Edge>;

public:
	explicit Edge(
		EdgeId const& id,
		NodeId const& tail,
		NodeId const& head,
		Weight const& weight = 1
	);

	EdgeId const& id() const;
	NodeId const& tail() const;
	NodeId const& head() const;
	Weight const& weight() const;

	NodeId const& opposite(NodeId const& node_id) const;

	bool is_valid() const;

	bool operator==(Edge const& rhs) const;

	std::string to_string() const final;

	json export_to_json() const;
	
private:

private:
	EdgeId /*const*/ _id;
	NodeId /*const*/ _tail;
	NodeId /*const*/ _head;
	Weight /*const*/ _weight;
};

} // namespace graph

#endif
