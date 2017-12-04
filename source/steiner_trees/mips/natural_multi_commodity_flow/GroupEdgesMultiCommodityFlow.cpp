#include "GroupEdgesMultiCommodityFlow.hpp"

namespace steiner_trees {

GroupEdgesMultiCommodityFlow::GroupEdgesMultiCommodityFlow(
	std::string const& name,
	graph::Graph const& graph,
	graph::Net::Vector const& nets,
	GroupEdges const& _group_edges,
	GroupMultiCommodityFlow const& _group_multi_commodity_flow
) :
	Group(name),
	_graph(graph),
	_nets(nets),
	_group_edges(_group_edges),
	_group_multi_commodity_flow(_group_multi_commodity_flow)
{}

void GroupEdgesMultiCommodityFlow::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_constraints(mip_model);
}

void GroupEdgesMultiCommodityFlow::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::EdgeId edge_id = 0; edge_id < _graph.num_edges(); ++edge_id) {
		for (graph::Net const& net : _nets) {
			for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
				mip::Constraint constraint = mip_model.create_constraint(
					name(),
					"connection : edge_id = " + std::to_string(edge_id)
					+ ", net_name = " + net.name()
					+ ", terminal_id = " + std::to_string(terminal_id)
				);

				constraint.add_variable(
					_group_edges.variables().get(edge_id, net.name()), -1
				);

				constraint.add_variable(
					_group_multi_commodity_flow.variables().get(edge_id, net.name(), terminal_id), 1
				);

				constraint.set_upper_bound(0);
			}
		}
	}
}

} // namespace steiner_trees
