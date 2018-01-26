#include "GroupEdges.hpp"

namespace steiner_trees {

GroupEdges::GroupEdges(
	std::string const& name,
	graph::Graph const& undirected_graph,
	graph::Net::Vector const& nets,
	bool binary,
	bool add_objective
) :
	Group(name),
	_undirected_graph(undirected_graph),
	_bidirected_graph(undirected_graph.bidirect()),
	_nets(nets),
	_binary(binary),
	_add_objective(add_objective)
{
	assert(not undirected_graph.is_directed());
}

void GroupEdges::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	if (_add_objective) {
		create_objective(mip_model);
	}
}

json GroupEdges::compute_solution() const
{
	json solution;

	for (graph::Net const& net : _nets) {
		for (graph::Edge const& edge : undirected_graph().edges()) {
			solution["undirected edges"][edge.to_string()] = undirected_edge_variables().solution_value(
				edge.id(), net.name()
			);
		}

		for (graph::Edge const& edge : bidirected_graph().edges()) {
			solution["bidirected edges"][edge.to_string()] = bidirected_edge_variables().solution_value(
				edge.id(), net.name()
			);
		}
	}

	return solution;
}

mip::VariableStorage<graph::EdgeId, graph::Net::Name> const& GroupEdges::undirected_edge_variables() const
{
	return _undirected_edge_variables;
}

mip::VariableStorage<graph::EdgeId, graph::Net::Name> const& GroupEdges::bidirected_edge_variables() const
{
	return _bidirected_edge_variables;
}

graph::Graph const& GroupEdges::undirected_graph() const
{
	return _undirected_graph;
}

graph::Graph const& GroupEdges::bidirected_graph() const
{
	return _bidirected_graph;
}

graph::Net::Vector const& GroupEdges::nets() const
{
	return _nets;
}

void GroupEdges::create_variables(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : undirected_graph().edges()) {
		for (graph::Net const& net : _nets) {
			if (_binary) {
				mip::MIPModel::Variable* variable = mip_model.create_binary_variable(
					name(), "undirected edge " + edge.to_string() + ", " + "net " + net.name()
				);
				_undirected_edge_variables.set(edge.id(), net.name(), variable);
			} else {
				mip::MIPModel::Variable* variable = mip_model.create_continuous_variable(
					name(), "undirected edge " + edge.to_string() + ", " + "net " + net.name(),
					0, 1
				);
				_undirected_edge_variables.set(edge.id(), net.name(), variable);
			}
		}
	}

	for (graph::Edge const& edge : bidirected_graph().edges()) {
		for (graph::Net const& net : _nets) {
			if (_binary) {
				mip::MIPModel::Variable* variable = mip_model.create_binary_variable(
					name(), "bidirected edge " + edge.to_string() + ", " + "net " + net.name()
				);
				_bidirected_edge_variables.set(edge.id(), net.name(), variable);
			} else {
				mip::MIPModel::Variable* variable = mip_model.create_continuous_variable(
					name(), "bidirected edge " + edge.to_string() + ", " + "net " + net.name(),
					0, 1
				);
				_bidirected_edge_variables.set(edge.id(), net.name(), variable);
			}
		}
	}
}

void GroupEdges::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : undirected_graph().edges()) {
		graph::Edge const edge_forward = bidirected_graph().edge(
			bidirected_graph().find_edge(edge.tail(), edge.head())
		);
		graph::Edge const edge_backward = bidirected_graph().edge(
			bidirected_graph().find_edge(edge.head(), edge.tail())
		);

		for (graph::Net const& net : _nets) {
			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"edge connection undirected edge " + edge.to_string()
				+ " and directed edge " + edge_forward.to_string()
				+ " and directed edge " + edge_backward.to_string()
				+ " and net " + net.name()
			);

			constraint.add_variable(bidirected_edge_variables().get(edge_forward.id(), net.name()), 1);
			constraint.add_variable(bidirected_edge_variables().get(edge_backward.id(), net.name()), 1);
			constraint.add_variable(undirected_edge_variables().get(edge.id(), net.name()), -1);

			constraint.set_upper_bound(0);
		}
	}
}

void GroupEdges::create_objective(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : undirected_graph().edges()) {
		for (graph::Net const& net : _nets) {
			mip_model.set_objective_coefficient(
				undirected_edge_variables().get(edge.id(), net.name()), edge.weight() * net.weight()
			);
		}
	}
}

} // namespace steiner_trees
