#include "SteinerTreeProblem.hpp"

namespace steiner_trees {

SteinerTreeProblem::SteinerTreeProblem(
	std::string const& name,
	graph::TerminalInstance const& terminal_instance,
	graph::Graph const& graph,
	graph::Net::Vector const& nets
) :
	_name(name),
	_terminal_instance(terminal_instance),
	_graph(graph),
	_nets(nets)
{}

std::string const& SteinerTreeProblem::name() const
{
	return _name;
}

graph::TerminalInstance const& SteinerTreeProblem::terminal_instance() const
{
	return _terminal_instance;
}

graph::Graph const& SteinerTreeProblem::graph() const
{
	return _graph;
}

graph::Net::Vector const& SteinerTreeProblem::nets() const
{
	return _nets;
}

json SteinerTreeProblem::export_to_json() const
{
	json result;

	result["graph"] = graph().export_to_json();

	for (graph::Net const& net : nets()) {
		result["nets"].push_back(net.export_to_json());
	}

	return result;
}

} // namespace steiner_trees
