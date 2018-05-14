#ifndef STEINER_TREES_GROUPBIDIRECTEDCUTSDUAL_HPP
#define STEINER_TREES_GROUPBIDIRECTEDCUTSDUAL_HPP

#include "../../../mip/Group.hpp"
#include "../../../mip/VariableStorage.hpp"
#include "../../../graph/Graph.hpp"
#include "../../../graph/Net.hpp"
#include "../natural_multi_commodity_flow/GroupDynamicGraph.hpp"

namespace steiner_trees {

class GroupBidirectedCutsDual : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupBidirectedCutsDual>;

public:
	explicit GroupBidirectedCutsDual(
		std::string const& name,
		graph::Graph const& undirected_graph,
		graph::Net::Vector const& nets
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	graph::Graph _bidirected_graph;
	graph::Net::Vector const& _nets;

	mip::VariableStorage<std::string> _variables;
};

} // namespace steiner_trees

#endif
