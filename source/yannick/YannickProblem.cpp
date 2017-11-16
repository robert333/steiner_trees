#include "YannickProblem.hpp"

namespace yannick {

YannickProblem::YannickProblem(
	std::string const& name,
	graph::Graph const& precedence_graph,
	double cycle_time,
	Index cycle_number,
	Index machine_number
) :
	_name(name),
	_precedence_graph(precedence_graph),
	_cycle_time(cycle_time),
	_cycle_number(cycle_number),
	_machine_number(machine_number)
{}

std::string const& YannickProblem::name() const
{
	return _name;
}

graph::Graph const& YannickProblem::precedence_graph() const
{
	return _precedence_graph;
}

double YannickProblem::cycle_time() const
{
	return _cycle_time;
}

Index YannickProblem::cycle_number() const
{
	return _cycle_number;
}

Index YannickProblem::machine_number() const
{
	return _machine_number;
}

bool YannickProblem::is_it_possible_to_process_the_tasks_by_one_machine(graph::Node::Vector const& nodes) const
{
	auto const sum_weight = [](graph::Weight weight, graph::Node const& node)->graph::Weight {
		return weight + node.weight();
	};

	return std::accumulate(nodes.begin(), nodes.end(), 0, sum_weight) <= cycle_time();
}

json YannickProblem::export_to_json() const
{
	return json();
}

} // namespace yannick
