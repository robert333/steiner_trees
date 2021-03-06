#include "GroupSteinerTreeCuts.hpp"

#include "../../SteinerTreeCut.hpp"

namespace steiner_trees {

GroupSteinerTreeCuts::GroupSteinerTreeCuts(
	std::string const& name,
	graph::Graph const& graph,
	graph::Net::Vector const& nets,
	GroupEdges const& group_edges
) :
	Group(name),
	_graph(graph),
	_nets(nets),
	_group_edges(group_edges)
{}

void GroupSteinerTreeCuts::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_constraints(mip_model);
}

void GroupSteinerTreeCuts::create_constraints(mip::MIPModel& mip_model)
{
	for (graph::Net const& net : _nets) {
		SteinerTreeCut steiner_tree_cut(_graph, net);

		do {
			mip::Constraint constraint = mip_model.create_constraint(
				name(),
				"SteinerTreeCut[" + steiner_tree_cut.to_string() + "]"
			);

			constraint.set_lower_bound(1);

			for (graph::EdgeId const& edge_id : steiner_tree_cut.compute_outgoing_edges()) {
				constraint.add_variable(_group_edges.bidirected_edge_variables().get(edge_id, net.name()), 1);
			}
		} while (steiner_tree_cut.next());
	}
}

} // namespace steiner_trees
