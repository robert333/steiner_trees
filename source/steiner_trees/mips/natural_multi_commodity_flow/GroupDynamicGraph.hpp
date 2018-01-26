#ifndef STEINER_TREES_GROUPDYNAMICGRAPH_HPP
#define STEINER_TREES_GROUPDYNAMICGRAPH_HPP

#include "GroupEdges.hpp"

namespace steiner_trees {

class GroupDynamicGraph : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupDynamicGraph>;

public:
	explicit GroupDynamicGraph(
		std::string const& name,
		GroupEdges const& _group_edges
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::EdgeId> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);

private:
	GroupEdges const& _group_edges;

	mip::VariableStorage<graph::EdgeId> _variables;
};

} // namespace steiner_trees

#endif
