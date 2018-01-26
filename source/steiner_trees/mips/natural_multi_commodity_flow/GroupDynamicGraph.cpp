#include "GroupDynamicGraph.hpp"

namespace steiner_trees {

GroupDynamicGraph::GroupDynamicGraph(
	std::string const& name,
	GroupEdges const& _group_edges
) :
	Group(name),
	_group_edges(_group_edges)
{}

void GroupDynamicGraph::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
}

json GroupDynamicGraph::compute_solution() const
{
	json solution;

	for (graph::Edge const& edge : _group_edges.undirected_graph().edges()) {
		solution["edges"][edge.to_string()] = _variables.solution_value(edge.id());
	}

	return solution;
}

mip::VariableStorage<graph::EdgeId> const& GroupDynamicGraph::variables() const
{
	return _variables;
}

void GroupDynamicGraph::create_variables(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : _group_edges.undirected_graph().edges()) {
		mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
			name(), "edge " + edge.to_string()
		);

		_variables.set(edge.id(), variable);
	}
}

void GroupDynamicGraph::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : _group_edges.undirected_graph().edges()) {
		for (graph::Net const& net : _group_edges.nets()) {
			mip::Constraint constraint = mip_model.create_constraint(
				name(), "dynamic edge " + edge.to_string()
			);

			constraint.add_variable(_variables.get(edge.id()), -1);
			constraint.add_variable(_group_edges.undirected_edge_variables().get(edge.id(), net.name()), 1);

			constraint.set_upper_bound(0);
		}
	}
}

} // namespace steiner_trees
