#include "GroupMultiCommodityOptimal.hpp"

namespace steiner_trees {

GroupMultiCommodityOptimal::GroupMultiCommodityOptimal(
	std::string const& name,
	GroupEdges const& group_edges,
	GroupMultiCommodityDual const& group_multi_commodity_dual
) :
	Group(name),
	_group_edges(group_edges),
	_group_multi_commodity_dual(group_multi_commodity_dual)
{}

void GroupMultiCommodityOptimal::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_constraints(mip_model);
}

void GroupMultiCommodityOptimal::create_constraints(mip::MIPModel& mip_model)
{
	mip::Constraint constraint = mip_model.create_constraint(name(), "optimal");

	constraint.set_upper_bound(0);

	for (graph::Net const& net : _group_edges.nets()) {
		for (graph::Edge const& edge : _group_edges.undirected_graph().edges()) {
			constraint.add_variable(
				_group_edges.undirected_edge_variables().get(edge.id(), net.name()), net.weight() * edge.weight()
			);
		}

		graph::Node const& root_node = _group_edges.undirected_graph().node(net.terminal(0));

		for (graph::TerminalId terminal_id = 1; terminal_id < net.num_terminals(); ++terminal_id) {
			constraint.add_variable(
				_group_multi_commodity_dual.variables().get(root_node.id(), net.name(), terminal_id), -1
			);
		}
	}
}

} // namespace steiner_trees
