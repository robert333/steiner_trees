#include "GroupBidirectedCutsDual.hpp"

#include "../../SteinerTreeCut.hpp"

namespace steiner_trees {

GroupBidirectedCutsDual::GroupBidirectedCutsDual(
	std::string const& name,
	graph::Graph const& undirected_graph,
	graph::Net::Vector const& nets
) :
	Group(name),
	_bidirected_graph(undirected_graph.bidirect()),
	_nets(nets)
{}

void GroupBidirectedCutsDual::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_constraints(mip_model);
	create_objective(mip_model);
}

json GroupBidirectedCutsDual::compute_solution() const
{
	json solution;

    for (graph::Net const& net : _nets) {
        SteinerTreeCut steiner_tree_cut(_bidirected_graph, net);

        do {
            solution[net.name()]["steiner_tree_cuts"].push_back(
                    {
                            {"steiner_tree_cut", steiner_tree_cut.to_string()},
                            {"value", _variables.solution_value(steiner_tree_cut.to_string())}
                    }
            );
        } while (steiner_tree_cut.next());
    }

	return solution;
}

void GroupBidirectedCutsDual::create_variables(mip::MIPModel& mip_model)
{
    for (graph::Net const& net : _nets) {
        SteinerTreeCut steiner_tree_cut(_bidirected_graph, net);

        do {
            mip::MIPModel::Variable* const variable = mip_model.create_continuous_variable(
                    name(),
                    steiner_tree_cut.to_string(),
                    0,
                    operations_research::MPSolver::infinity()
            );

            _variables.set(steiner_tree_cut.to_string(), variable);
        } while (steiner_tree_cut.next());
    }
}

void GroupBidirectedCutsDual::create_constraints(mip::MIPModel& mip_model)
{
    for (graph::Edge const& edge : _bidirected_graph.edges()) {
        for (graph::Net const& net : _nets) {
            mip::Constraint constraint = mip_model.create_constraint(
                    name(),
                    "edge " + edge.to_string()
            );

            constraint.set_upper_bound(edge.weight());

            SteinerTreeCut steiner_tree_cut(_bidirected_graph, net);

            do {
                if (helper::contains(steiner_tree_cut.compute_outgoing_edges(), edge.id())) {
                    constraint.add_variable(_variables.get(steiner_tree_cut.to_string()), 1);
                }
            } while (steiner_tree_cut.next());
        }
    }
}

void GroupBidirectedCutsDual::create_objective(mip::MIPModel& mip_model)
{
	for (graph::Net const& net : _nets) {
        SteinerTreeCut steiner_tree_cut(_bidirected_graph, net);

        do {
            mip_model.set_objective_coefficient(_variables.get(steiner_tree_cut.to_string()), 1);
        } while (steiner_tree_cut.next());
    }
}

} // namespace steiner_trees
