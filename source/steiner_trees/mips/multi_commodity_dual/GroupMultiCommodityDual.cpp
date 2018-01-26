#include "GroupMultiCommodityDual.hpp"

#include "../../../Logger.hpp"
#include "../../../mip/MIPSolver.hpp"
#include "../../../graph/algorithms/Dijkstra.hpp"

namespace steiner_trees {

GroupMultiCommodityDual::GroupMultiCommodityDual(
	std::string const& name,
	graph::Graph const& graph,
	graph::Net::Vector const& nets,
	GroupDynamicGraph const& group_dynamic_graph,
	bool add_objective
) :
	Group(name),
	_graph(graph),
	_nets(nets),
	_group_dynamic_graph(group_dynamic_graph),
	_add_objective(add_objective)
{}

void GroupMultiCommodityDual::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	if (_add_objective) {
		create_objective(mip_model);
	}
}

json GroupMultiCommodityDual::compute_solution() const
{
	json solution;

	for (graph::Node const& node : _graph.nodes()) {
		for (graph::Net const& net : _nets) {
			for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
				solution["nodes"].push_back(
					{
						{"node", node.id()},
						{"net", net.name()},
						{"terminal", terminal_id},
						{"value", variables().solution_value(node.id(), net.name(), terminal_id)}
					}
				);
			}
		}
	}

	return solution;
}

mip::VariableStorage<graph::NodeId, graph::Net::Name, graph::TerminalId> const& GroupMultiCommodityDual::variables() const
{
	return _variables;
}

void GroupMultiCommodityDual::create_variables(mip::MIPModel& mip_model)
{
	mip::Value sum_edge_weights = 0;

	for (graph::Edge const& edge : _graph.edges()) {
		sum_edge_weights += edge.weight();
	}

	for (graph::Node const& node : _graph.nodes()) {
		for (graph::Net const& net : _nets) {
			for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
				mip::Value const lower_bound = 0;
				mip::Value const upper_bound = node == _graph.node(net.terminal(terminal_id)) ? 0 : _graph.num_nodes() - 1;

				mip::MIPModel::Variable* const variable = mip_model.create_continuous_variable(
					name(),
					"node = " + node.to_string() + ", "
					+ "net_name = " + net.name() + ", "
					+ "terminal_id = " + std::to_string(terminal_id),
					lower_bound,
					upper_bound
				);
				_variables.set(node.id(), net.name(), terminal_id, variable);
			}
		}
	}
}

void GroupMultiCommodityDual::create_constraints(mip::MIPModel& mip_model)
{
//	create_nmc_constraints(mip_model);
	create_emc_constraints(mip_model);
}

void GroupMultiCommodityDual::create_objective(mip::MIPModel& mip_model)
{
	for (graph::Net const& net : _nets) {
		graph::Node const& root_node = _graph.node(net.terminal(0));

		for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
			mip_model.set_objective_coefficient(_variables.get(root_node.id(), net.name(), terminal_id), 1);
		}
	}
}

void GroupMultiCommodityDual::create_nmc_constraints(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : _graph.edges()) {
		graph::Node const& tail = _graph.node(edge.tail());
		graph::Node const& head = _graph.node(edge.head());

		for (graph::Net const& net : _nets) {
			mip::Constraint cost_bound_constraint = mip_model.create_constraint(
				name(),
				"cost bound for edge " + edge.to_string() + " and net " + net.name()
			);

			cost_bound_constraint.set_upper_bound(edge.weight());

			for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
				mip::MIPModel::Variable* const tail_variable = _variables.get(tail.id(), net.name(), terminal_id);
				mip::MIPModel::Variable* const head_variable = _variables.get(head.id(), net.name(), terminal_id);

				mip::Value const lower_bound = 0;
				mip::Value const upper_bound = std::max(
					tail_variable->ub() - head_variable->lb(),
					head_variable->ub() - tail_variable->lb()
				);

				mip::MIPModel::Variable* const norm_variable = mip_model.create_continuous_variable(
					name(),
					"norm of node " + tail.to_string() + " and node " + head.to_string()
					+ " for net " + net.name() + " and terminal " + std::to_string(terminal_id),
					lower_bound,
					upper_bound
				);

				mip::Constraint norm_constraint_1 = mip_model.create_constraint(
					name(),
					"1 norm of node " + tail.to_string() + " and node " + head.to_string()
					+ " for net " + net.name() + " and terminal " + std::to_string(terminal_id)
				);

				mip::Constraint norm_constraint_2 = mip_model.create_constraint(
					name(),
					"2 norm of node " + tail.to_string() + " and node " + head.to_string()
					+ " for net " + net.name() + " and terminal " + std::to_string(terminal_id)
				);

				norm_constraint_1.add_variable(tail_variable, 1);
				norm_constraint_1.add_variable(head_variable, -1);
				norm_constraint_1.add_variable(norm_variable, -1);
				norm_constraint_1.set_upper_bound(0);

				norm_constraint_2.add_variable(tail_variable, -1);
				norm_constraint_2.add_variable(head_variable, 1);
				norm_constraint_2.add_variable(norm_variable, -1);
				norm_constraint_2.set_upper_bound(0);

				cost_bound_constraint.add_variable(norm_variable, 1);
			}
		}
	}
}

void GroupMultiCommodityDual::create_emc_constraints(mip::MIPModel& mip_model)
{
	for (graph::Edge const& edge : _graph.edges()) {
		graph::Node const& tail = _graph.node(edge.tail());
		graph::Node const& head = _graph.node(edge.head());

		for (graph::Net const& net : _nets) {
			mip::Constraint cost_bound_constraint_1 = mip_model.create_constraint(
				name(),
				"1 cost bound for edge " + edge.to_string() + " and net " + net.name()
			);

			mip::Constraint cost_bound_constraint_2 = mip_model.create_constraint(
				name(),
				"2 cost bound for edge " + edge.to_string() + " and net " + net.name()
			);

			cost_bound_constraint_1.set_upper_bound(edge.weight());
			cost_bound_constraint_2.set_upper_bound(edge.weight());

			for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
				mip::MIPModel::Variable* const tail_variable = _variables.get(tail.id(), net.name(), terminal_id);
				mip::MIPModel::Variable* const head_variable = _variables.get(head.id(), net.name(), terminal_id);

				mip::Value const lower_bound_1 = 0;
				mip::Value const upper_bound_1 = tail_variable->ub() - head_variable->lb();

				mip::Value const lower_bound_2 = 0;
				mip::Value const upper_bound_2 = head_variable->ub() - tail_variable->lb();

				mip::MIPModel::Variable* const upper_bound_variable_1 = mip_model.create_continuous_variable(
					name(),
					"upper bound of tail " + tail.to_string() + " and head " + head.to_string()
					+ " for net " + net.name() + " and terminal " + std::to_string(terminal_id),
					lower_bound_1,
					upper_bound_1
				);

				mip::MIPModel::Variable* const upper_bound_variable_2 = mip_model.create_continuous_variable(
					name(),
					"upper bound of head " + tail.to_string() + " and tail " + head.to_string()
					+ " for net " + net.name() + " and terminal " + std::to_string(terminal_id),
					lower_bound_2,
					upper_bound_2
				);

				mip::Constraint upper_bound_constraint_1 = mip_model.create_constraint(
					name(),
					"upper bound of tail " + tail.to_string() + " and head " + head.to_string()
					+ " for net " + net.name() + " and terminal " + std::to_string(terminal_id)
				);

				mip::Constraint upper_bound_constraint_2 = mip_model.create_constraint(
					name(),
					"upper bound of head " + tail.to_string() + " and tail " + head.to_string()
					+ " for net " + net.name() + " and terminal " + std::to_string(terminal_id)
				);

				upper_bound_constraint_1.add_variable(tail_variable, 1);
				upper_bound_constraint_1.add_variable(head_variable, -1);
				upper_bound_constraint_1.add_variable(upper_bound_variable_1, -1);
				upper_bound_constraint_1.set_upper_bound(0);

				upper_bound_constraint_2.add_variable(tail_variable, -1);
				upper_bound_constraint_2.add_variable(head_variable, 1);
				upper_bound_constraint_2.add_variable(upper_bound_variable_2, -1);
				upper_bound_constraint_2.set_upper_bound(0);

				cost_bound_constraint_1.add_variable(upper_bound_variable_1, 1);
				cost_bound_constraint_2.add_variable(upper_bound_variable_2, 1);
			}

			cost_bound_constraint_1.upper_bound_condition_on(_group_dynamic_graph.variables().get(edge.id()));
			cost_bound_constraint_2.upper_bound_condition_on(_group_dynamic_graph.variables().get(edge.id()));
		}
	}
}

} // namespace steiner_trees
