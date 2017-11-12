#ifndef STEINER_TREES_GROUPMULTICOMMODITYFLOW_HPP
#define STEINER_TREES_GROUPMULTICOMMODITYFLOW_HPP

#include "../../../mip/Group.hpp"
#include "../../../mip/VariableStorage.hpp"
#include "../../../graph/Graph.hpp"
#include "../../../graph/Net.hpp"

namespace steiner_trees {

class GroupMultiCommodityFlow : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupMultiCommodityFlow>;

public:
	explicit GroupMultiCommodityFlow(
		std::string const& name,
		graph::Graph const& graph,
		graph::Net::Vector const& nets
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	mip::VariableStorage<graph::EdgeId, graph::Net::Name, graph::TerminalId> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);

private:
	graph::Graph const& _graph;
	graph::Net::Vector const& _nets;

	mip::VariableStorage<graph::EdgeId, graph::Net::Name, graph::TerminalId> _variables;
};

} // namespace steiner_trees

#endif
