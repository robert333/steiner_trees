#include "GroupCommonFlow.hpp"

namespace steiner_trees {

GroupCommonFlow::GroupCommonFlow(
	std::string const& name,
	GroupEdges const& _group_edges,
	GroupMultiCommodityFlow const& _group_multi_commodity_flow,
	graph::Net const& net
) :
	Group(name),
	_group_edges(_group_edges),
	_group_multi_commodity_flow(_group_multi_commodity_flow),
	_net(net)
{}

void GroupCommonFlow::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	create_objective(mip_model);
}

json GroupCommonFlow::compute_solution() const
{
	json solution;

	helper::PowerSetIterator power_set_iterator(_net.num_terminals() - 1, 2);

	do {
		helper::PowerSetIterator::Set const terminal_set = power_set_iterator.compute_current_subset();

		for (graph::Edge const& edge : _group_edges.terminal_instance().bidirected_graph().edges()) {
			solution["common_flow"][edge.to_string()] = _common_flow_variables.solution_value(edge.id(), terminal_set);
		}

		for (graph::Node const& node : _group_edges.terminal_instance().bidirected_graph().nodes()) {
			solution["common_flow_rejoin_appearance"][node.to_string()] =
				_common_flow_rejoin_appearance_variables.solution_value(node.id(), terminal_set);
			solution["common_flow_rejoin"][node.to_string()] =
				_common_flow_rejoin_variables.solution_value(node.id(), terminal_set);
		}
	} while (power_set_iterator.next());

	return solution;
}

void GroupCommonFlow::create_variables(mip::MIPModel& mip_model)
{
	helper::PowerSetIterator power_set_iterator(_net.num_terminals() - 1, 2);

	do {
		helper::PowerSetIterator::Set const terminal_set = power_set_iterator.compute_current_subset();

		for (graph::Edge const& edge : _group_edges.terminal_instance().bidirected_graph().edges()) {
			mip::MIPModel::Variable* const variable = mip_model.create_continuous_variable(
				name(),
				"common flow on edge " + edge.to_string() + " for terminals " + helper::to_string(terminal_set),
				0, 1
			);

			_common_flow_variables.set(edge.id(), terminal_set, variable);
		}

		for (graph::Node const& node : _group_edges.terminal_instance().bidirected_graph().nodes()) {
			_common_flow_rejoin_appearance_variables.set(
				node.id(), terminal_set,
				mip_model.create_binary_variable(
					name(),
					"common flow rejoin appearance on node " + node.to_string()
					+ " for terminals " + helper::to_string(terminal_set)
				)
			);

			_common_flow_rejoin_variables.set(
				node.id(), terminal_set,
				mip_model.create_continuous_variable(
					name(),
					"common flow rejoin on node " + node.to_string()
					+ " for terminals " + helper::to_string(terminal_set),
					0, 1
				)
			);
		}
	} while (power_set_iterator.next());
}

void GroupCommonFlow::create_constraints(mip::MIPModel& mip_model)
{
	helper::PowerSetIterator power_set_iterator(_net.num_terminals() - 1, 2);

	do {
		helper::PowerSetIterator::Set const terminal_set = power_set_iterator.compute_current_subset();

		for (graph::Edge const& edge : _group_edges.terminal_instance().bidirected_graph().edges()) {
			mip::Constraint constraint_lower_bound = mip_model.create_constraint(
				name(),
				"common flow lower bound on edge " + edge.to_string()
				+ " for terminals " + helper::to_string(terminal_set)
			);

			constraint_lower_bound.set_upper_bound(0);

			constraint_lower_bound.add_variable(
				_common_flow_variables.get(edge.id(), terminal_set), -1
			);
			constraint_lower_bound.add_variable(
				_group_edges.bidirected_edge_variables().get(edge.id(), _net.name()), -1
			);

			for (graph::TerminalId const& terminal_id : terminal_set) {
				constraint_lower_bound.add_variable(
					_group_multi_commodity_flow.variables().get(edge.id(), _net.name(), terminal_id + 1), 1
				);

				mip::Constraint constraint_upper_bound = mip_model.create_constraint(
					name(),
					"common flow upper bound of terminal "
					+ std::to_string(terminal_id) + " on edge " + edge.to_string()
					+ " for terminals " + helper::to_string(terminal_set)
				);

				constraint_upper_bound.add_variable(
					_common_flow_variables.get(edge.id(), terminal_set), 1
				);
				constraint_upper_bound.add_variable(
					_group_multi_commodity_flow.variables().get(edge.id(), _net.name(), terminal_id + 1), -1
				);

				constraint_upper_bound.set_upper_bound(0);
			}
		}

		for (graph::Node const& node : _group_edges.terminal_instance().bidirected_graph().nodes()) {
			if (node.id() == _net.terminal(0)) {
				continue;
			}

			mip::Constraint constraint_lower_bound = mip_model.create_constraint(
				name(),
				"lower_bound common flow rejoin at node " + node.to_string() + " for terminals " + helper::to_string(terminal_set)
			);

			mip::Constraint constraint_upper_bound = mip_model.create_constraint(
				name(),
				"upper_bound common flow rejoin at node " + node.to_string() + " for terminals " + helper::to_string(terminal_set)
			);

			constraint_lower_bound.set_lower_bound(-1);
			constraint_upper_bound.set_upper_bound(0);

			constraint_lower_bound.add_variable(_common_flow_rejoin_variables.get(node.id(), terminal_set), -1);
			constraint_lower_bound.add_variable(_common_flow_rejoin_appearance_variables.get(node.id(), terminal_set), -1);

			constraint_upper_bound.add_variable(_common_flow_rejoin_appearance_variables.get(node.id(), terminal_set), -1);

			for (graph::EdgeId const& edge_id : node.outgoing_edges()) {
				constraint_lower_bound.add_variable(_common_flow_variables.get(edge_id, terminal_set), 1);
				constraint_upper_bound.add_variable(_common_flow_variables.get(edge_id, terminal_set), 1);
			}

			for (graph::EdgeId const& edge_id : node.incoming_edges()) {
				constraint_lower_bound.add_variable(_common_flow_variables.get(edge_id, terminal_set), -1);
				constraint_upper_bound.add_variable(_common_flow_variables.get(edge_id, terminal_set), -1);
			}

//			constraint_upper_bound.upper_bound_condition_on(
//				_common_flow_rejoin_appearance_variables.get(node.id(), terminal_set), 0
//			);
//			constraint_lower_bound.lower_bound_condition_on(
//				_common_flow_rejoin_appearance_variables.get(node.id(), terminal_set), 1
//			);

			mip::Constraint constraint_appearance = mip_model.create_constraint(
				name(),
				"appearance common flow rejoin at node " + node.to_string() + " for terminals " + helper::to_string(terminal_set)
			);

			constraint_appearance.set_upper_bound(0);

			constraint_appearance.add_variable(_common_flow_rejoin_variables.get(node.id(), terminal_set), 1);
			constraint_appearance.add_variable(_common_flow_rejoin_appearance_variables.get(node.id(), terminal_set), -1);
		}
	} while (power_set_iterator.next());
}

void GroupCommonFlow::create_objective(mip::MIPModel& mip_model)
{
	helper::PowerSetIterator power_set_iterator(_net.num_terminals() - 1, 2);

	do {
		helper::PowerSetIterator::Set const terminal_set = power_set_iterator.compute_current_subset();

		for (graph::Node const& node : _group_edges.terminal_instance().bidirected_graph().nodes()) {
			if (node.id() == _net.terminal(0)) {
				continue;
			}

			mip_model.set_objective_coefficient(_common_flow_rejoin_variables.get(node.id(), terminal_set), 1);
		}
	} while (power_set_iterator.next());
}

} // namespace steiner_trees
