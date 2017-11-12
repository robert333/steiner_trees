#ifndef STEINER_TREES_STEINERTREECUT_HPP
#define STEINER_TREES_STEINERTREECUT_HPP

#include "SteinerTreeProblem.hpp"

namespace steiner_trees {

class SteinerTreeCut {
public:
	using SharedPtr = std::shared_ptr<SteinerTreeCut>;
	using Vector = std::vector<SteinerTreeCut>;

public:
	explicit SteinerTreeCut(graph::Graph const& graph, graph::Net const& net);

	bool next();

	bool is_valid() const;

	std::vector<graph::NodeId> compute_nodes_within_cut() const;
	std::vector<graph::EdgeId> compute_outgoing_edges() const;

	std::string to_string() const;

private:
	bool is_zero_characteristic_vector();
	void next_characteristic_vector();

private:
	graph::Graph _graph;
	graph::Net _net;

	std::vector<bool> _characteristic_vector;
};

} // namespace steiner_trees

#endif
