#ifndef STEINER_TREES_GROUPEDGESMULTICOMMODITYFLOW_HPP
#define STEINER_TREES_GROUPEDGESMULTICOMMODITYFLOW_HPP

#include "GroupEdges.hpp"
#include "GroupMultiCommodityFlow.hpp"

namespace steiner_trees {

class GroupEdgesMultiCommodityFlow : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupEdgesMultiCommodityFlow>;

public:
	explicit GroupEdgesMultiCommodityFlow(
		std::string const& name,
		graph::Graph const& graph,
		graph::Net::Vector const& nets,
		GroupEdges const& _group_edges,
		GroupMultiCommodityFlow const& _group_multi_commodity_flow
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

private:
	void create_constraints(mip::MIPModel& mip_model);

private:
	graph::Graph const& _graph;
	graph::Net::Vector const& _nets;

	GroupEdges const& _group_edges;
	GroupMultiCommodityFlow const& _group_multi_commodity_flow;

	mip::VariableStorage<graph::EdgeId, graph::Net::Name, graph::TerminalId> _variables;
};

} // namespace steiner_trees

#endif
