#include "GroupBidirectedMultiCommodityCommonFlow.hpp"

namespace steiner_trees {

GroupBidirectedMultiCommodityCommonFlow::GroupBidirectedMultiCommodityCommonFlow(
	std::string const& name,
	GroupEdges const& group_edges
) :
	Group(name),
	_group_edges(group_edges)
{}

void GroupBidirectedMultiCommodityCommonFlow::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
}

void GroupBidirectedMultiCommodityCommonFlow::create_variables(mip::MIPModel& mip_model)
{
	graph::TerminalInstance const& terminal_instance = _group_edges.terminal_instance();

	for (helper::PowerSetIterator::Cardinality subset_cardinality = 0; subset_cardinality <= 2; ++subset_cardinality) {
		helper::PowerSetIterator power_set_iterator(terminal_instance.sink_terminal_ids().size(), subset_cardinality);

		do {
			std::vector<graph::TerminalId> const sink_terminal_set = power_set_iterator.compute_subvector(
				terminal_instance.sink_terminal_ids(),
				power_set_iterator.compute_current_subset()
			);

			for (graph::Edge const& edge : terminal_instance.bidirected_graph().edges()) {
				mip::MIPModel::Variable* const variable = mip_model.create_continuous_variable(
					name(),
					"edge " + edge.to_string() + ", sink_terminal_set " + helper::to_string(sink_terminal_set),
					0,
					1
				);

				_variables.set(edge.id(), sink_terminal_set, variable);
			}
		} while (power_set_iterator.next());
	}
}

void GroupBidirectedMultiCommodityCommonFlow::create_constraints(mip::MIPModel& mip_model)
{
	graph::TerminalInstance const& terminal_instance = _group_edges.terminal_instance();

	for (graph::TerminalId const& sink_terminal_id : terminal_instance.sink_terminal_ids()) {
		for (graph::Node const& node : terminal_instance.bidirected_graph().nodes()) {
			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"balance node : node " + node.to_string()
				+ ", sink_terminal_id = " + std::to_string(sink_terminal_id)
			);

			for (graph::EdgeId const& outgoing_edge_id : node.outgoing_edges()) {
				constraint.add_variable(_variables.get(outgoing_edge_id, {sink_terminal_id}), 1);
			}

			for (graph::EdgeId const& incoming_edge_id : node.incoming_edges()) {
				constraint.add_variable(_variables.get(incoming_edge_id, {sink_terminal_id}), -1);
			}

			if (node.id() == terminal_instance.root_terminal()) {
				constraint.set_lower_bound(1);
				constraint.set_upper_bound(1);
			} else if (node.id() == terminal_instance.terminal(sink_terminal_id)) {
				constraint.set_lower_bound(-1);
				constraint.set_upper_bound(-1);
			} else {
				constraint.set_lower_bound(0);
				constraint.set_upper_bound(0);
			}
		}

		for (graph::Edge const& edge : terminal_instance.bidirected_graph().edges()) {
			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"connection empty set : edge " + edge.to_string()
				+ ", sink_terminal_id = " + std::to_string(sink_terminal_id)
			);

			constraint.add_variable(_variables.get(edge.id(), {}), -1);
			constraint.add_variable(_variables.get(edge.id(), {sink_terminal_id}), 1);

			constraint.set_upper_bound(0);
		}
	}

	for (graph::Edge const& edge : terminal_instance.bidirected_graph().edges()) {
		mip::Constraint constraint = mip_model.create_constraint(
			name(), "connection GroupEdges : edge " + edge.to_string()
		);

		constraint.add_variable(_group_edges.bidirected_edge_variables().get(edge.id(), _group_edges.nets().front().name()), -1);
		constraint.add_variable(_variables.get(edge.id(), {}), 1);

		constraint.set_upper_bound(0);
	}

	create_common_flow_constraints(mip_model);
}

void GroupBidirectedMultiCommodityCommonFlow::create_common_flow_constraints(mip::MIPModel& mip_model)
{
	graph::TerminalInstance const& terminal_instance = _group_edges.terminal_instance();

	helper::PowerSetIterator power_set_iterator(terminal_instance.sink_terminal_ids().size(), 2);

	do {
		std::vector<graph::TerminalId> const sink_terminal_set = power_set_iterator.compute_subvector(
			terminal_instance.sink_terminal_ids(),
			power_set_iterator.compute_current_subset()
		);

		for (graph::Edge const& edge : terminal_instance.bidirected_graph().edges()) {
			for (graph::TerminalId const& sink_terminal_id : sink_terminal_set) {
				mip::Constraint constraint = mip_model.create_constraint(
					name(),
					"connection common flow lower bound : edge " + edge.to_string()
					+ ", sink_terminal_id = " + std::to_string(sink_terminal_id)
					+ ", sink_terminal_set = " + helper::to_string(sink_terminal_set)
				);

				constraint.add_variable(_variables.get(edge.id(), {sink_terminal_id}), -1);
				constraint.add_variable(_variables.get(edge.id(), sink_terminal_set), 1);

				constraint.set_upper_bound(0);
			}

			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"connection common flow upper bound : edge " + edge.to_string()
				+ ", sink_terminal_set = " + helper::to_string(sink_terminal_set)
			);

			for (graph::TerminalId const& sink_terminal_id : sink_terminal_set) {
				constraint.add_variable(_variables.get(edge.id(), {sink_terminal_id}), 1);
			}

			constraint.add_variable(_variables.get(edge.id(), {}), -1);
			constraint.add_variable(_variables.get(edge.id(), sink_terminal_set), -1);

			constraint.set_upper_bound(0);
		}

		for (graph::Node const& node : terminal_instance.bidirected_graph().nodes()) {
			if (node.id() == terminal_instance.root_terminal()) {
				continue;
			}

			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"balance common flow : node " + node.to_string()
				+ ", sink_terminal_set = " + helper::to_string(sink_terminal_set)
			);

			for (graph::EdgeId const& outgoing_edge_id : node.outgoing_edges()) {
				constraint.add_variable(_variables.get(outgoing_edge_id, sink_terminal_set), 1);
			}

			for (graph::EdgeId const& incoming_edge_id : node.incoming_edges()) {
				constraint.add_variable(_variables.get(incoming_edge_id, sink_terminal_set), -1);
			}

			constraint.set_upper_bound(0);
		}
	} while (power_set_iterator.next());
}

} // namespace steiner_trees
