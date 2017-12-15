#include "GroupMultiCommodityFlow.hpp"

namespace steiner_trees {

GroupMultiCommodityFlow::GroupMultiCommodityFlow(
	std::string const& name,
	graph::Graph const& graph,
	graph::Net::Vector const& nets
) :
	Group(name),
	_graph(graph),
	_nets(nets)
{
	assert(graph.is_directed());
}

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
	for (graph::EdgeId edge_id = 0; edge_id < _graph.num_edges(); ++edge_id) {
		for (graph::Net const& net : _nets) {
			for (graph::TerminalId terminal_id = 0; terminal_id < net.num_terminals(); ++terminal_id) {
				mip::MIPModel::Variable* const variable = mip_model.create_binary_variable(
					name(),
					"edge_id = " + std::to_string(edge_id) + ", "
					+ "net_name = " + net.name() + ", "
					+ "terminal_id = " + std::to_string(terminal_id)
				);
				_variables.set(edge_id, net.name(), terminal_id, variable);
			}
		}
	}
}

void GroupMultiCommodityFlow::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Net const& net : _nets) {
		for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
			for (graph::NodeId node_id = 0; node_id < _graph.num_nodes(); ++node_id) {
				graph::Node const& node = _graph.node(node_id);

				mip::Constraint constraint = mip_model.create_constraint(
					name(),
					"balance node : node_id = " + std::to_string(node_id) + ", "
					+ "net_name = " + net.name() + ", "
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

				if (net.terminal(0) == node_id) {
					constraint.set_lower_bound(1);
					constraint.set_upper_bound(1);
				} else if (net.terminal(terminal_id) == node_id) {
					constraint.set_lower_bound(-1);
					constraint.set_upper_bound(-1);
				} else {
					constraint.set_lower_bound(0);
					constraint.set_upper_bound(0);
				}
			}
		}
	}
}

} // namespace steiner_trees
