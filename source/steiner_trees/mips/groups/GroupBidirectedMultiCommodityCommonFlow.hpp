#ifndef STEINER_TREES_BIDIRECTEDMULTICOMMODITYCOMMONFLOW_HPP
#define STEINER_TREES_BIDIRECTEDMULTICOMMODITYCOMMONFLOW_HPP

#include "../natural_multi_commodity_flow/GroupEdges.hpp"
#include "../../../helper/PowerSetIterator.hpp"

namespace steiner_trees {

class GroupBidirectedMultiCommodityCommonFlow : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupBidirectedMultiCommodityCommonFlow>;

public:
	explicit GroupBidirectedMultiCommodityCommonFlow(
		std::string const& name,
		GroupEdges const& group_edges
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);

	void create_common_flow_constraints(mip::MIPModel& mip_model);

private:
	GroupEdges const& _group_edges;

	mip::VariableStorage<graph::EdgeId, std::vector<graph::TerminalId>> _variables;
};

} // namespace steiner_trees

#endif
