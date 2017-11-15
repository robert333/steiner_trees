#ifndef STEINER_TREES_GROUPEDGES_HPP
#define STEINER_TREES_GROUPEDGES_HPP

#include "../../../mip/Group.hpp"
#include "../../../mip/VariableStorage.hpp"
#include "../../../graph/Graph.hpp"
#include "../../../graph/Net.hpp"

namespace steiner_trees {

class GroupEdges : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupEdges>;

public:
	explicit GroupEdges(
		std::string const& name,
		graph::Graph const& graph,
		graph::Net::Vector const& nets
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::EdgeId, graph::Net::Name> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	graph::Graph const& _graph;
	graph::Net::Vector const& _nets;

	mip::VariableStorage<graph::EdgeId, graph::Net::Name> _variables;
};

} // namespace steiner_trees

#endif
