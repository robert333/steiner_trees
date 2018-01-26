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
		graph::Graph const& undirected_graph,
		graph::Net::Vector const& nets,
		bool binary = true,
		bool add_objective = true
	);

	void create_variables_constraints_and_objective(mip::MIPModel& mip_model) final;

	json compute_solution() const final;

	mip::VariableStorage<graph::EdgeId, graph::Net::Name> const& undirected_edge_variables() const;
	mip::VariableStorage<graph::EdgeId, graph::Net::Name> const& bidirected_edge_variables() const;

	graph::Graph const& undirected_graph() const;
	graph::Graph const& bidirected_graph() const;
	graph::Net::Vector const& nets() const;

private:
	void create_variables(mip::MIPModel& mip_model);
	void create_constraints(mip::MIPModel& mip_model);
	void create_objective(mip::MIPModel& mip_model);

private:
	graph::Graph const& _undirected_graph;
	graph::Graph _bidirected_graph;

	graph::Net::Vector const& _nets;

	bool _binary;
	bool _add_objective;

	mip::VariableStorage<graph::EdgeId, graph::Net::Name> _undirected_edge_variables;
	mip::VariableStorage<graph::EdgeId, graph::Net::Name> _bidirected_edge_variables;
};

} // namespace steiner_trees

#endif
