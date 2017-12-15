#include "GroupEdges.hpp"
#include "../../../Logger.hpp"

namespace steiner_trees {

GroupEdges::GroupEdges(
	std::string const& name,
	graph::Graph const& graph,
	graph::Net::Vector const& nets,
	bool binary
) :
	Group(name),
	_graph(graph),
	_nets(nets),
	_binary(binary)
{
	assert(graph.is_directed());
}

void GroupEdges::create_variables_constraints_and_objective(mip::MIPModel& mip_model)
{
	create_variables(mip_model);
	create_objective(mip_model);
}

json GroupEdges::compute_solution() const
{
	json solution;

	Logger::logger() << "\nSolution GroupEdges\n";

	for (graph::EdgeId edge_id = 0; edge_id < _graph.num_edges(); ++edge_id) {
		graph::Edge const& edge = _graph.edge(edge_id);

		for (graph::Net const& net : _nets) {
			solution["edges"][std::to_string(edge_id)] = variables().solution_value(edge_id, net.name());

			Logger::logger() << edge << " (" << std::to_string(edge_id) << "), " << net << " : "
					         << variables().solution_value(edge_id, net.name()) << "\n";
		}
	}

	Logger::logger() << "\n";

	return solution;
}

mip::VariableStorage<graph::EdgeId, graph::Net::Name> const& GroupEdges::variables() const
{
	return _variables;
}

graph::Graph const& GroupEdges::graph() const
{
	return _graph;
}

graph::Net::Vector const& GroupEdges::nets() const
{
	return _nets;
}

void GroupEdges::create_variables(mip::MIPModel& mip_model)
{
	for (graph::EdgeId edge_id = 0; edge_id < _graph.num_edges(); ++edge_id) {
		for (graph::Net const& net : _nets) {
			mip::MIPModel::Variable* variable = nullptr;

			if (_binary) {
				variable = mip_model.create_binary_variable(
					name(),
					"edge_id = " + std::to_string(edge_id) + ", "
					+ "net_name = " + net.name()
				);
			} else {
				variable = mip_model.create_continuous_variable(
					name(),
					"edge_id = " + std::to_string(edge_id) + ", "
					+ "net_name = " + net.name(),
					0,
					1
				);
			}

			_variables.set(edge_id, net.name(), variable);
		}
	}
}

void GroupEdges::create_objective(mip::MIPModel& mip_model)
{
	for (graph::EdgeId edge_id = 0; edge_id < _graph.num_edges(); ++edge_id) {
		graph::Edge const& edge = _graph.edge(edge_id);

		for (graph::Net const& net : _nets) {
			mip_model.set_objective_coefficient(_variables.get(edge_id, net.name()), edge.weight() * net.weight());
		}
	}
}

} // namespace steiner_trees
