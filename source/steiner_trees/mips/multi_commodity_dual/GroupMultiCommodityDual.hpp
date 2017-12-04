#ifndef STEINER_TREES_GROUPMULTICOMMODITYDUAL_HPP
#define STEINER_TREES_GROUPMULTICOMMODITYDUAL_HPP

#include "../../../mip/Group.hpp"
#include "../../../mip/VariableStorage.hpp"
#include "../../../graph/Graph.hpp"
#include "../../../graph/Net.hpp"

namespace steiner_trees {

class GroupMultiCommodityDual : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupMultiCommodityDual>;

public:
	explicit GroupMultiCommodityDual(
		std::string const& name,
		graph::Graph const& graph,
		graph::Net::Vector const& nets
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::NodeId, graph::Net::Name, graph::TerminalId> const& variables() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

	void create_nmc_constraints(mip::MIPModel& mip_model);
	void create_emc_constraints(mip::MIPModel& mip_model);

private:
	graph::Graph const& _graph;
	graph::Net::Vector const& _nets;

	mip::VariableStorage<graph::NodeId, graph::Net::Name, graph::TerminalId> _variables;
};

} // namespace steiner_trees

#endif
