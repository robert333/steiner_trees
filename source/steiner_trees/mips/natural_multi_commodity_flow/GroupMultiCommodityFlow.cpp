#include "GroupMultiCommodityFlow.hpp"

namespace steiner_trees {

GroupMultiCommodityFlow::GroupMultiCommodityFlow(
	std::string const& name,
	GroupEdges const& group_edges,
	bool binary
) :
	Group(name),
	_group_edges(group_edges),
	_binary(binary)
{}

void GroupMultiCommodityFlow::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
}

mip::VariableStorage<graph::EdgeId, graph::Net::Name, graph::TerminalId> const& GroupMultiCommodityFlow::variables() const
{
	return _variables;
}

void GroupMultiCommodityFlow::create_variables(mip::MIPModel& mip_model)
{
	for (graph::Net const& net : _group_edges.nets()) {
		for (graph::Edge const& edge : _group_edges.terminal_instance().bidirected_graph().edges()) {
			for (graph::TerminalId terminal_id = 0; terminal_id < net.num_terminals(); ++terminal_id) {
				if (_binary) {
					mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
						name(),
						"edge " + edge.to_string() + ", " + "net " + net.name() + ", "
						+ "terminal_id = " + std::to_string(terminal_id)
					);
					_variables.set(edge.id(), net.name(), terminal_id, variable);
				} else {
					mip::MIPModel::Variable* const variable = mip_model.create_continuous_variable(
						name(),
						"edge " + edge.to_string() + ", " + "net " + net.name() + ", "
						+ "terminal_id = " + std::to_string(terminal_id),
						0, 1
					);
					_variables.set(edge.id(), net.name(), terminal_id, variable);
				}
			}
		}
	}
}

void GroupMultiCommodityFlow::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Net const& net : _group_edges.nets()) {
		for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
			for (graph::Node const& node : _group_edges.terminal_instance().bidirected_graph().nodes()) {
				mip::Constraint constraint = mip_model.create_constraint(
					name(),
					"balance node : node " + node.to_string() + ", "
					+ "net " + net.name() + ", "
					+ "terminal_id = " + std::to_string(terminal_id)
				);

				for (graph::EdgeId const& outgoing_edge_id : node.outgoing_edges()) {
//					graph::Edge const& outgoing_edge = _graph.edge(outgoing_edge_id);
//					graph::NodeId const& outgoing_node = outgoing_edge.opposite(node_id);

					constraint.add_variable(_variables.get(outgoing_edge_id, net.name(), terminal_id), 1);
				}

				for (graph::EdgeId const& incoming_edge_id : node.incoming_edges()) {
//					graph::Edge const& incoming_edge = _graph.edge(incoming_edge_id);
//					graph::NodeId const& incoming_node = incoming_edge.opposite(node_id);

					constraint.add_variable(_variables.get(incoming_edge_id, net.name(), terminal_id), -1);
				}

				if (net.terminal(0) == node.id()) {
					constraint.set_lower_bound(1);
					constraint.set_upper_bound(1);
				} else if (net.terminal(terminal_id) == node.id()) {
					constraint.set_lower_bound(-1);
					constraint.set_upper_bound(-1);
				} else {
					constraint.set_lower_bound(0);
					constraint.set_upper_bound(0);
				}
			}

			for (graph::Edge const& edge : _group_edges.terminal_instance().bidirected_graph().edges()) {
				mip::Constraint constraint = mip_model.create_constraint(
					name(),
					"connection : edge " + edge.to_string()
					+ ", net_name = " + net.name()
					+ ", terminal_id = " + std::to_string(terminal_id)
				);

				constraint.add_variable(_group_edges.bidirected_edge_variables().get(edge.id(), net.name()), -1);
				constraint.add_variable(_variables.get(edge.id(), net.name(), terminal_id), 1);

				constraint.set_upper_bound(0);
			}
		}
	}
}

} // namespace steiner_trees
