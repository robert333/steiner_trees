#ifndef STEINER_TREES_GROUPSIMPLEXEMBEDDING_HPP
#define STEINER_TREES_GROUPSIMPLEXEMBEDDING_HPP

#include "../../../mip/Group.hpp"
#include "../../../mip/VariableStorage.hpp"
#include "../../../graph/Graph.hpp"
#include "../../../graph/Net.hpp"

namespace steiner_trees {

class GroupSimplexEmbedding : public mip::Group {
public:
	using SharedPtr = std::shared_ptr<GroupSimplexEmbedding>;

public:
	explicit GroupSimplexEmbedding(
		std::string const& name,
		graph::Graph const& graph,
		graph::Net const& net
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	graph::Graph const& _graph;
	graph::Net const& _net;

	mip::VariableStorage<int> _simplex_variable;
	mip::VariableStorage<graph::NodeId, graph::TerminalId> _node_variables;
	mip::VariableStorage<graph::EdgeId, graph::TerminalId> _edge_variables;
};

} // namespace steiner_trees

#endif
