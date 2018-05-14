#include "GroupSimplexEmbedding.hpp"
#include "../../../Logger.hpp"

namespace steiner_trees {

GroupSimplexEmbedding::GroupSimplexEmbedding(
	std::string const& name,
	graph::Graph const& graph,
	graph::Net const& net
) :
	Group(name),
	_graph(graph),
	_net(net)
{
	assert(not graph.is_directed());
}

void GroupSimplexEmbedding::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	create_objective(mip_model);
}

json GroupSimplexEmbedding::compute_solution() const
{
	json solution;

	solution["lambda"] = _simplex_variable.solution_value(0);

	for (graph::Node const& node : _graph.nodes()) {
		std::vector<double> terminal_solution_values;

		for (graph::TerminalId terminal_id = 0; terminal_id < _net.num_terminals(); ++terminal_id) {
			terminal_solution_values.push_back(_node_variables.solution_value(node.id(), terminal_id));
		}

		solution["nodes"].push_back(
			{
				{"node",     node.to_string()},
				{"position", terminal_solution_values}
			}
		);
	}

	return solution;
}

void GroupSimplexEmbedding::create_variables(mip::MIPModel& mip_model)
{
	mip::MIPModel::Variable* simplex_variable = mip_model.create_continuous_variable(
		name(), "simplex_variable", 0, operations_research::MPSolver::infinity()
	);

	_simplex_variable.set(0, simplex_variable);

	for (graph::TerminalId terminal_id = 0; terminal_id < _net.num_terminals(); ++terminal_id) {
		for (graph::Node const& node : _graph.nodes()) {
			mip::MIPModel::Variable* variable = mip_model.create_continuous_variable(
				name(), "node " + node.to_string() + ", terminal " + std::to_string(terminal_id),
				0, operations_research::MPSolver::infinity()
			);

			_node_variables.set(node.id(), terminal_id, variable);
		}

		for (graph::Edge const& edge : _graph.edges()) {
			mip::MIPModel::Variable* variable = mip_model.create_continuous_variable(
				name(), "edge " + edge.to_string() + ", terminal " + std::to_string(terminal_id),
				0, operations_research::MPSolver::infinity()
			);

			_edge_variables.set(edge.id(), terminal_id, variable);
		}
	}
}

void GroupSimplexEmbedding::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Node const& node : _graph.nodes()) {
		mip::Constraint constraint = mip_model.create_constraint(
			name(), "simplex embedding for node " + node.to_string()
		);

		constraint.add_variable(_simplex_variable.get(0), -1);

		for (graph::TerminalId terminal_id = 0; terminal_id < _net.num_terminals(); ++terminal_id) {
			constraint.add_variable(_node_variables.get(node.id(), terminal_id), 1);
		}

		constraint.set_lower_bound(0);

//		if (_net.is_terminal(node.id())) {
			constraint.set_upper_bound(0);
//		}
	}

	for (graph::Edge const& edge : _graph.edges()) {
		mip::Constraint edge_constraint = mip_model.create_constraint(
			name(), "cost bound for edge " + edge.to_string()
		);

		edge_constraint.set_upper_bound(2 * edge.weight());

		for (graph::TerminalId terminal_id = 0; terminal_id < _net.num_terminals(); ++terminal_id) {
			edge_constraint.add_variable(_edge_variables.get(edge.id(), terminal_id), 1);

			mip::Constraint edge_terminal_constraint_1 = mip_model.create_constraint(
				name(), "1. norm bound for edge " + edge.to_string() + " and terminal " + std::to_string(terminal_id)
			);
			mip::Constraint edge_terminal_constraint_2 = mip_model.create_constraint(
				name(), "2. norm bound for edge " + edge.to_string() + " and terminal " + std::to_string(terminal_id)
			);

			edge_terminal_constraint_1.set_upper_bound(0);
			edge_terminal_constraint_2.set_upper_bound(0);

			edge_terminal_constraint_1.add_variable(_edge_variables.get(edge.id(), terminal_id), -1);
			edge_terminal_constraint_2.add_variable(_edge_variables.get(edge.id(), terminal_id), -1);

			edge_terminal_constraint_1.add_variable(_node_variables.get(edge.tail(), terminal_id), 1);
			edge_terminal_constraint_1.add_variable(_node_variables.get(edge.head(), terminal_id), -1);

			edge_terminal_constraint_2.add_variable(_node_variables.get(edge.tail(), terminal_id), -1);
			edge_terminal_constraint_2.add_variable(_node_variables.get(edge.head(), terminal_id), 1);
		}
	}
}

void GroupSimplexEmbedding::create_objective(mip::MIPModel& mip_model)
{
	mip_model.set_objective_coefficient(_simplex_variable.get(0), -1);

	for (graph::TerminalId terminal_id = 0; terminal_id < _net.num_terminals(); ++terminal_id) {
		mip_model.set_objective_coefficient(_node_variables.get(_net.terminal(terminal_id), terminal_id), 1);
	}
}

} // namespace steiner_trees
