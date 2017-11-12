#ifndef STEINER_TREES_GROUPSTEINERTREECUTS_HPP
#define STEINER_TREES_GROUPSTEINERTREECUTS_HPP

#include "GroupEdges.hpp"

namespace steiner_trees {

class GroupSteinerTreeCuts : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupSteinerTreeCuts>;

public:
	explicit GroupSteinerTreeCuts(
		std::string const& name,
		graph::Graph const& graph,
		graph::Net::Vector const& nets,
		GroupEdges const& group_edges
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

private:
	void create_constraints(mip::MIPModel& mip_model);

private:
	graph::Graph const& _graph;
	graph::Net::Vector const& _nets;

	GroupEdges const& _group_edges;
};

} // namespace steiner_trees

#endif
