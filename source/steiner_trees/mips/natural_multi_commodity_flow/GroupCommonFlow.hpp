#ifndef STEINER_TREES_GROUPCOMMONFLOW_HPP
#define STEINER_TREES_GROUPCOMMONFLOW_HPP

#include "GroupEdges.hpp"
#include "GroupMultiCommodityFlow.hpp"
#include "../../../helper/PowerSetIterator.hpp"

namespace steiner_trees {

class GroupCommonFlow : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupCommonFlow>;

public:
	explicit GroupCommonFlow(
		std::string const& name,
		GroupEdges const& _group_edges,
		GroupMultiCommodityFlow const& _group_multi_commodity_flow,
		graph::Net const& net
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	GroupEdges const& _group_edges;
	GroupMultiCommodityFlow const& _group_multi_commodity_flow;

	graph::Net const& _net;

	mip::VariableStorage<graph::EdgeId, helper::PowerSetIterator::Set> _common_flow_variables;
	mip::VariableStorage<graph::NodeId, helper::PowerSetIterator::Set> _common_flow_rejoin_appearance_variables;
	mip::VariableStorage<graph::NodeId, helper::PowerSetIterator::Set> _common_flow_rejoin_variables;
};

} // namespace steiner_trees

#endif
